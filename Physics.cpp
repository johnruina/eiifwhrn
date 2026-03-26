#include "Physics.h"
#include "Debug.h"

void Physics::Step(float dt) {
	for (p* p : objects) {
		if (p->velocity == true) {
			p->force += p->mass * gravity
				//+ -p->linearvelocity/2.0f
				;
			//p->angularvelocity += -(p->angularvelocity / 2.0f)/p->mass*dt;
			p->linearvelocity += p->force / p->mass * dt;
		}
	}
	ResolveCollisions(dt);

	for (p* p : objects) {
		if (p->velocity == true) {

			p->pointer->t.TranslateBy(p->linearvelocity * dt);
			p->pointer->t.RotateByQuaternion(glm::quat(p->angularvelocity * dt));

			p->force = glm::vec3(0.0f);
		}
	}
}

void Physics::Resolve(Collision* c)
{
    p* a = c->ObjA;
    p* b = c->ObjB;
    glm::vec3 collisionnormal = c->CN;
    glm::vec3 relativecola = c->POI - a->pointer->t.GetTranslation();
    glm::vec3 relativecolb = c->POI - b->pointer->t.GetTranslation();

    constexpr float onedividedbytwelve = 0.08333333333f;
    constexpr float restitution = 0.4f;

    // Inertia tensor A
    glm::vec3 sqa = a->pointer->t.GetScale() * a->pointer->t.GetScale();
    glm::mat3 invIA(0.0f);
    invIA[0][0] = 1.0f / (onedividedbytwelve * a->mass * (sqa.y + sqa.z));
    invIA[1][1] = 1.0f / (onedividedbytwelve * a->mass * (sqa.x + sqa.z));
    invIA[2][2] = 1.0f / (onedividedbytwelve * a->mass * (sqa.x + sqa.y));
    glm::mat3 rA = glm::mat3(a->pointer->t.GetRotationMatrix());
    invIA = rA * invIA * glm::transpose(rA);

    // Inertia tensor B
    glm::vec3 sqb = b->pointer->t.GetScale() * b->pointer->t.GetScale();
    glm::mat3 invIB(0.0f);
    invIB[0][0] = 1.0f / (onedividedbytwelve * b->mass * (sqb.y + sqb.z));
    invIB[1][1] = 1.0f / (onedividedbytwelve * b->mass * (sqb.x + sqb.z));
    invIB[2][2] = 1.0f / (onedividedbytwelve * b->mass * (sqb.x + sqb.y));
    glm::mat3 rB = glm::mat3(b->pointer->t.GetRotationMatrix());
    invIB = rB * invIB * glm::transpose(rB);

    float invMassA = a->velocity ? 1.0f / a->GetMass() : 0.0f;
    float invMassB = b->velocity ? 1.0f / b->GetMass() : 0.0f;

    // Velocity at contact point for each body
    glm::vec3 velA = a->linearvelocity + glm::cross(a->angularvelocity, relativecola);
    glm::vec3 velB = b->linearvelocity + glm::cross(b->angularvelocity, relativecolb);
    glm::vec3 relativeVelocity = velA - velB;

    float relativeVelAlongNormal = glm::dot(relativeVelocity, collisionnormal);

    // Already separating
    if (relativeVelAlongNormal > 0.0f) return;

    glm::vec3 angularTermA = invIA * glm::cross(glm::cross(relativecola, collisionnormal), relativecola);
    glm::vec3 angularTermB = invIB * glm::cross(glm::cross(relativecolb, collisionnormal), relativecolb);
    float angularFactor = glm::dot(angularTermA + angularTermB, collisionnormal);

    float impulse = -(1.0f + restitution) * relativeVelAlongNormal
        / (invMassA + invMassB + angularFactor);

    // Normal impulse
    glm::vec3 impulseVec = impulse * collisionnormal;
    if (a->velocity) {
        a->linearvelocity += invMassA * impulseVec;
        a->angularvelocity += invIA * glm::cross(relativecola, impulseVec);
    }
    if (b->velocity) {
        b->linearvelocity -= invMassB * impulseVec;
        b->angularvelocity -= invIB * glm::cross(relativecolb, impulseVec);
    }

    // Friction impulse
    glm::vec3 tangent = relativeVelocity - relativeVelAlongNormal * collisionnormal;
    if (glm::length(tangent) > 1e-6f) {
        tangent = glm::normalize(tangent);
        float frictionMag = -glm::dot(relativeVelocity, tangent)
            / (invMassA + invMassB
                + glm::dot(invIA * glm::cross(glm::cross(relativecola, tangent), relativecola)
                    + invIB * glm::cross(glm::cross(relativecolb, tangent), relativecolb),
                    tangent));
        constexpr float mu = 0.5f;
        frictionMag = glm::clamp(frictionMag, -mu * impulse, mu * impulse);
        glm::vec3 frictionImpulse = frictionMag * tangent;
        if (a->velocity) {
            a->linearvelocity += invMassA * frictionImpulse;
            a->angularvelocity += invIA * glm::cross(relativecola, frictionImpulse);
        }
        if (b->velocity) {
            b->linearvelocity -= invMassB * frictionImpulse;
            b->angularvelocity -= invIB * glm::cross(relativecolb, frictionImpulse);
        }
    }
}