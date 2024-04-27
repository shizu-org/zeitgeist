

| direction? | position? |  color                                  |
|------------|-----------|------------------------------------------
|     no     |    no     | $l_c$                                   |
|     yes    |    no     | $\max(\vec{n} \cdot \vec{l}_d), 0) l_c$ |
|     no     |    yes    | $\max(\vec{n} \cdot \vec{l}_d), 0) l_c$ |

## Ambient Light
If a light has no direction and no position then it is considered as
emitting the same amount of light on any fragment. The final color
is hence the light color $\vec{l}_c$.

## Diffuse Light
A diffuse light has a direction and no position. It emits an amount of
light on any fragment that depends on the angle between the fragment
normal and the inverse light direction. The light emitted on a surface
is given by
$$
\max(\hat{f}_n \cdot -\hat{l}_d,0) \vec{l}_c
$$
Whenever the angle between the vectors is greater than 0 and smaller than 180 $\max(\hat{f}_n \cdot -\vhat{l}_d,0)$ evaluates to a value between $(0,1)$.
In all other cases, it evaluates to a value between $[-1,0]$. In these cases the maximum function maps the value to $0$.

## Specular Directional Light
A specular light without position and direction is called a specular
directional light. It reflects light back from the fragment to the
viewer depending on the light direction and the direction from the
fragmen to the viewer.

The specular point light either uses the Phong or the Blinn-Phong lighting model.

## Specular Point Light
A specular light with a position and a direction is called a specular
point light. It reflects light back from the fragment to the viewer
depending on the direction from the light to the fragment and the
direction from the fragment to the viewer.

The specular point light either uses the Phong or the Blinn-Phong lighting model.