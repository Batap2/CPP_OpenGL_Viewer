#ifndef RAYH
#define RAYH

class ray
{
    public:
        __device__ ray() {}
        __device__ ray(const glm::vec3& a, const glm::vec3& b) { o = a; d = b; }
        __device__ glm::vec3 origin() const       { return o; }
        __device__ glm::vec3 direction()const    { return d; }
        __device__ glm::vec3 point_at_parameter(float t) const { return o + t*d; }

        glm::vec3 o;
        glm::vec3 d;
};

#endif
