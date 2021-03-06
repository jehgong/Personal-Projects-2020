#include "triangle.h"

#include "CGL/CGL.h"
#include "GL/glew.h"

namespace CGL { namespace StaticScene {

Triangle::Triangle(const Mesh* mesh, size_t v1, size_t v2, size_t v3) :
    mesh(mesh), v1(v1), v2(v2), v3(v3) { }

BBox Triangle::get_bbox() const {

  Vector3D p1(mesh->positions[v1]), p2(mesh->positions[v2]), p3(mesh->positions[v3]);
  BBox bb(p1);
  bb.expand(p2); 
  bb.expand(p3);
  return bb;

}

bool withinOne(double v) {
  if (v >= 0 && v <= 1) {
    return true;
  }
  else {
    return false;
  }
}

bool Triangle::intersect(const Ray& r) const {
  
  // Part 1, Task 3: implement ray-triangle intersection
  Vector3D p1(mesh->positions[v1]), p2(mesh->positions[v2]), p3(mesh->positions[v3]);

  Vector3D E1 = p2 - p1;
  Vector3D E2 = p3 - p1;
  Vector3D S = r.o - p1;
  Vector3D S1 = cross(r.d, E2);
  Vector3D S2 = cross(S, E1);
  double coef = 1.0 / dot(S1, E1);
  double t = coef * dot(S2, E2);
  double b1 = coef * dot(S1, S);
  double b2 = coef * dot(S2, r.d);

  if (withinOne(b1) && withinOne(b2) && (b1 + b2 <= 1) && t >= r.min_t && t <= r.max_t) {
    r.max_t = t;
    return true;
  }
  return false;
}

bool insideTriangle(Vector3D p, Vector3D p1, Vector3D p2, Vector3D p3, Vector3D n) {
    Vector3D e1 = p2 - p1;
    Vector3D e2 = p3 - p2;
    Vector3D e3 = p1 - p3;
    Vector3D c1 = p - p1;
    Vector3D c2 = p - p2;
    Vector3D c3 = p - p3;
    if (dot(n, cross(e1, c1)) > 0 && dot(n, cross(e2, c2)) > 0 && dot(n, cross(e3, c3)) > 0) {
      return true;
    }
    return false;
}


bool Triangle::intersect(const Ray& r, Intersection *isect) const {
  
  // Part 1, Task 3: 
  // implement ray-triangle intersection. When an intersection takes
  // place, the Intersection data should be updated accordingly
  Vector3D p1(mesh->positions[v1]), p2(mesh->positions[v2]), p3(mesh->positions[v3]);
  Vector3D n1(mesh->normals[v1]), n2(mesh->normals[v2]), n3(mesh->normals[v3]);

  Vector3D E1 = p2 - p1;
  Vector3D E2 = p3 - p1;
  Vector3D S = r.o - p1;
  Vector3D S1 = cross(r.d, E2);
  Vector3D S2 = cross(S, E1);
  double coef = 1.0 / dot(S1, E1);
  double t = coef * dot(S2, E2);
  double b1 = coef * dot(S1, S);
  double b2 = coef * dot(S2, r.d);

  if (withinOne(b1) && withinOne(b2) && (b1 + b2 <= 1) && t >= r.min_t && t <= r.max_t) {
    r.max_t = t;

    // double b3 = 1 - b1 - b2;
    Vector3D N = (1 - b1 - b2) * n1 + b1 * n2 + b2 * n3;

    isect->t = t;
    isect->primitive = this;
    isect->n = N;
    isect->bsdf = get_bsdf();

    return true;
  }
  return false;

  // if (withinOne(b1) && withinOne(b2) && (b1 + b2 <= 1) && t >= r.min_t && t <= r.max_t) {
  //   // r.max_t = t;
  //   double b3 = 1.0 - b1 - b2;
  //   Vector3D N = b1 * n1 + b2 * n2 + b3 * n3;

  //   // if (dot(N, r.d) == 0.0) {
  //   //   return false;  // ray and plane are parallel
  //   // }
  //   // if (t < 0) {
  //   //   return false;  // triangle is behind
  //   // }
  //   r.max_t = t;
  //   isect->t = t;
  //   isect->primitive = this;
  //   isect->n = N;
  //   isect->bsdf = get_bsdf();
  //   return true;

  //   // intersect point
  // double b3 = 1.0 - b1 - b2;
  // Vector3D N = b1 * n1 + b2 * n2 + b3 * n3;
  // Vector3D P = r.o + t * r.d;
  // Vector3D e1 = p2 - p1;
  // Vector3D e2 = p3 - p2;
  // Vector3D e3 = p1 - p3;
  // Vector3D c0 = P - p1;
  // Vector3D c1 = P - p2;
  // Vector3D c2 = P - p3;
  // // insideTriangle(Vector3D p, Vector3D p1, Vector3D p2, Vector3D p3, Vector3D n) 
  // if (insideTriangle(P, p1, p2, p3, N)) {
  //   r.max_t = t;
  //   isect->t = t;
  //   isect->primitive = this;
  //   isect->n = N;
  //   isect->bsdf = get_bsdf();

  //   return true;
  // }
  // return false;
  
}

void Triangle::draw(const Color& c) const {
  glColor4f(c.r, c.g, c.b, c.a);
  glBegin(GL_TRIANGLES);
  glVertex3d(mesh->positions[v1].x,
             mesh->positions[v1].y,
             mesh->positions[v1].z);
  glVertex3d(mesh->positions[v2].x,
             mesh->positions[v2].y,
             mesh->positions[v2].z);
  glVertex3d(mesh->positions[v3].x,
             mesh->positions[v3].y,
             mesh->positions[v3].z);
  glEnd();
}

void Triangle::drawOutline(const Color& c) const {
  glColor4f(c.r, c.g, c.b, c.a);
  glBegin(GL_LINE_LOOP);
  glVertex3d(mesh->positions[v1].x,
             mesh->positions[v1].y,
             mesh->positions[v1].z);
  glVertex3d(mesh->positions[v2].x,
             mesh->positions[v2].y,
             mesh->positions[v2].z);
  glVertex3d(mesh->positions[v3].x,
             mesh->positions[v3].y,
             mesh->positions[v3].z);
  glEnd();
}



} // namespace StaticScene
} // namespace CGL
