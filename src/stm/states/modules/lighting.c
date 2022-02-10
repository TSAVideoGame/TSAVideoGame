#include <stdio.h>
#include <JEL/jel.h>
#include "core/gll/gll.h"
#include "components/components.h"
#include "resm/resm.h"
#include "core/core.h"

extern JEL_Entity player;
extern struct Camera camera;

static void set_point_lights(unsigned int *shader)
{
  char buffer[50];

  struct JEL_Query q;
  JEL_QUERY(q, PointLight);
  for (unsigned int t = 0; t < q.count; ++t) {
    struct PointLightIt light;
    JEL_IT(light, q.tables[t], PointLight);

    for (unsigned int i = 1; i < q.tables[t]->count; ++i) {
      sprintf(buffer, "point_lights[%d].constant", (int) light.n[i]);
      glUniform1f(glGetUniformLocation(*shader, buffer), light.constant[i]);
      sprintf(buffer, "point_lights[%d].linear", (int) light.n[i]); 
      glUniform1f(glGetUniformLocation(*shader, buffer), light.linear[i]);
      sprintf(buffer, "point_lights[%d].quadratic", (int) light.n[i]); 
      glUniform1f(glGetUniformLocation(*shader, buffer), light.quadratic[i]);
    }
  }
  JEL_query_destroy(&q);
}

static void update_point_lights(void)
{
  struct Position pos;
  JEL_GET(player, Position, &pos);

  unsigned int *shader = JIN_resm_get("sprite_shader");
  glUseProgram(*shader);
  char buffer[50];

  struct JEL_Query q;
  JEL_QUERY(q, Position, PointLight);

  for (unsigned int t = 0; t < q.count; ++t) {
    struct PositionIt pos; JEL_IT(pos, q.tables[t], Position);
    struct PointLightIt light; JEL_IT(light, q.tables[t], PointLight);

    for (unsigned int i = 1; i < q.tables[t]->count; ++i) {
      sprintf(buffer, "point_lights[%d].position", (int) light.n[i]);
      glUniform2f(glGetUniformLocation(*shader, buffer), (float) (pos.x[i] - camera.x + 16), (float) (pos.y[i] - camera.y + 16));
    }
  }

  JEL_query_destroy(&q);
}

