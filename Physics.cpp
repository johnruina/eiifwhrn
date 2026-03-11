#include "Physics.h"
#include "Debug.h"

void Physics::Resolve(Collision* c)
{
	p* a = c->ObjA;
	p* b = c->ObjB;

	glm::vec3 ascale = a->pointer->t.GetScale();
	glm::vec3 bscale = b->pointer->t.GetScale();

	glm::vec3 relativevelocity = a->linearvelocity - b->linearvelocity;
	glm::vec3 collisionnormal = c->CN;
	//collisionnormal = {0.0f,1.0f,0.0f};
	glm::vec3 relativecola = c->POI - a->pointer->t.GetTranslation();
	glm::vec3 relativecolb = c->POI - b->pointer->t.GetTranslation();


	constexpr float onedividedbytwelve = 0.08333333333f;

	glm::vec3 sqa = a->pointer->t.GetScale() * a->pointer->t.GetScale();
	glm::mat3 inverseinertiatensora(0.0f);
	inverseinertiatensora[0][0] = onedividedbytwelve * a->mass * (sqa.y + sqa.z);
	inverseinertiatensora[1][1] = onedividedbytwelve * a->mass * (sqa.x + sqa.z);
	inverseinertiatensora[2][2] = onedividedbytwelve * a->mass * (sqa.x + sqa.y);
	glm::mat3 r = glm::mat3(a->pointer->t.GetRotationMatrix());
	inverseinertiatensora = r * glm::inverse(inverseinertiatensora) * glm::transpose(r);

	glm::vec3 sqb = b->pointer->t.GetScale() * b->pointer->t.GetScale();
	glm::mat3 inverseinertiatensorb(0.0f);
	inverseinertiatensorb[0][0] = onedividedbytwelve * b->mass * (sqb.y + sqb.z);
	inverseinertiatensorb[1][1] = onedividedbytwelve * b->mass * (sqb.x + sqb.z);
	inverseinertiatensorb[2][2] = onedividedbytwelve * b->mass * (sqb.x + sqb.y);
	r = glm::mat3(b->pointer->t.GetRotationMatrix());
	inverseinertiatensorb = r * glm::inverse(inverseinertiatensorb) * glm::transpose(r);

	float inversemass1 = 1.0f / a->GetMass();
	float inversemass2 = 1.0f / b->GetMass();

	float totalvelocity = -glm::dot((1 + a->restitution * b->restitution) * relativevelocity, collisionnormal);

	float impulse = glm::max(
		totalvelocity / 
		(inversemass1// + inversemass2
		+ glm::dot(
		inverseinertiatensora * glm::cross(glm::cross(relativecola, collisionnormal), relativecola) 
		+ inverseinertiatensorb * glm::cross(glm::cross(relativecolb, collisionnormal), relativecolb)
		, collisionnormal))
		,0.0f);

	float frictionfactor = 0.5f;
	glm::vec3 frictionvector = glm::cross(glm::cross(collisionnormal, relativevelocity), collisionnormal);
	if (a->velocity) {
		a->linearvelocity += inversemass1 * impulse * collisionnormal * 60.0f;
		a->angularvelocity += inverseinertiatensora * impulse * glm::cross(relativecola, collisionnormal) / 4.0f;
	}

	if (b->velocity) {
		b->linearvelocity -= inversemass2 * impulse * collisionnormal * 60.0f;
		b->angularvelocity -= inverseinertiatensorb * impulse * glm::cross(relativecolb, collisionnormal) / 4.0f;
	}
}
