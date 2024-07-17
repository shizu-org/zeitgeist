#include "Player.h"

#include "Matrix4F32.h"

static void
Player_visit
  (
    Shizu_State2* state,
    Player* self
  );

static Shizu_ObjectTypeDescriptor const Player_Type = {
  .preDestroyType = NULL,
  .postCreateType = NULL,
  .visitType = NULL,
  .size = sizeof(Player),
  .finalize = NULL,
  .visit = (Shizu_OnVisitCallback*) & Player_visit,
  .dispatchSize = sizeof(Player_Dispatch),
  .dispatchInitialize = NULL,
  .dispatchUninitialize = NULL,
};

Shizu_defineObjectType("Zeitgeist.Player", Player, Shizu_Object);

static void
Player_visit
  (
    Shizu_State2* state,
    Player* self
  )
{
  if (self->position) {
    Shizu_Gc_visitObject(Shizu_State2_getState1(state), Shizu_State2_getGc(state), (Shizu_Object*)self->position);
  }
  if (self->positionSpeed) {
    Shizu_Gc_visitObject(Shizu_State2_getState1(state), Shizu_State2_getGc(state), (Shizu_Object*)self->positionSpeed);
  }
}

Player*
Player_create
  (
    Shizu_State2* state
  )
{
  Shizu_Type* type = Player_getType(state);
  Player* self = (Player*)Shizu_Gc_allocateObject(state, sizeof(Player));
  self->position = Vector3F32_create(state, 0.f, 0.f, 0.f);
  self->positionSpeed = Vector3F32_create(state, 0.f, 0.f, 0.f);
  self->rotationY = 0.f;
  self->rotationYSpeed = 0.f;

  self->strafeLeftDown = false;
  self->strafeRightDown = false;
  self->moveForwardDown = false;
  self->moveBackwardDown = false;
  self->turnLeftDown = false ;
  self->turnRightDown = false;

  ((Shizu_Object*)self)->type = type;
  return self;
}

void
Player_onKeyboardKeyMessage
  (
    Shizu_State2* state,
    Player* self,
    KeyboardKeyMessage* message
  )
{
  switch (KeyboardKeyMessage_getKey(state, message)) {
    case KeyboardKey_Q: {
      self->turnLeftDown = KeyboardKey_Action_Pressed == KeyboardKeyMessage_getAction(state, message);
    } break;
    case KeyboardKey_E: {
      self->turnRightDown = KeyboardKey_Action_Pressed == KeyboardKeyMessage_getAction(state, message);
    } break;
    case KeyboardKey_W:
    case KeyboardKey_Up: {
      self->moveForwardDown = KeyboardKey_Action_Pressed == KeyboardKeyMessage_getAction(state, message);
    } break;
    case KeyboardKey_S:
    case KeyboardKey_Down: {
      self->moveBackwardDown = KeyboardKey_Action_Pressed == KeyboardKeyMessage_getAction(state, message);
    } break;
    case KeyboardKey_A:
    case KeyboardKey_Left: {
      self->strafeLeftDown = KeyboardKey_Action_Pressed == KeyboardKeyMessage_getAction(state, message);
    } break;
    case KeyboardKey_D:
    case KeyboardKey_Right: {
      self->strafeRightDown = KeyboardKey_Action_Pressed == KeyboardKeyMessage_getAction(state, message);
    } break;
  };
}

void
Player_update
  (
    Shizu_State2* state,
    Player* self,
    Shizu_Float32 tick
  )
{
  self->positionSpeed = Vector3F32_create(state, 0.f, 0.f, 0.f);
  if (self->strafeLeftDown != self->strafeRightDown) {
    if (self->strafeLeftDown) {
      Vector3F32* speed = Vector3F32_create(state, -1.f, 0.f, 0.f);
      self->positionSpeed = Vector3F32_add(state, self->positionSpeed, speed);
    } else {
      Vector3F32* speed = Vector3F32_create(state, +1.f, 0.f, 0.f);
      self->positionSpeed = Vector3F32_add(state, self->positionSpeed, speed);
    }
  }
  if (self->moveForwardDown != self->moveBackwardDown) {
    if (self->moveForwardDown) {
      Vector3F32* speed = Vector3F32_create(state, 0.f, 0.f, -1.f);
      self->positionSpeed = Vector3F32_add(state, self->positionSpeed, speed);
    } else {
      Vector3F32* speed = Vector3F32_create(state, 0.f, 0.f, +1.f);
      self->positionSpeed = Vector3F32_add(state, self->positionSpeed, speed);
    }
  }
  self->rotationYSpeed = 0.f;
  if (self->turnLeftDown != self->turnRightDown) {
    if (self->turnLeftDown) {
      self->rotationYSpeed += +1.f;
    } else {
      self->rotationYSpeed += -1.f;
    }
  }

  // update rotation.
  self->rotationY += 0.005f * self->rotationYSpeed * tick;

  // update position.
  idlib_vector_3_f32 v = self->positionSpeed->v;
  if (idlib_vector_3_f32_normalize(&v, &v)) {
    Matrix4F32* rotationY = Matrix4F32_createRotateY(state, self->rotationY);
    idlib_matrix_4x4_3f_transform_direction(&v, &rotationY->m, &v);
    Shizu_Float32 speed = 0.0001f * tick;
    v.e[0] *= speed;
    v.e[1] *= speed;
    v.e[2] *= speed;
    idlib_vector_3_f32_add(&self->position->v, &self->position->v, &v);
  }
}
