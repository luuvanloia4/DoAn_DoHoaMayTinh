#include "PerlinNoise.h"

PerlinNoise::PerlinNoise() {
    for (int i = 0; i < PerlinPermutationSize; i++) {
        m_P[256 + i] = m_P[i] = PerlinPermutation[i];
    }
}

float PerlinNoise::Noise(float x, float y, float z) {
    int X = (int)floor(x) & 255;
    int Y = (int)floor(y) & 255;
    int Z = (int)floor(z) & 255;

    x -= floor(x);
    y -= floor(y);
    z -= floor(z);

    float u = Fade(x);
    float v = Fade(y);
    float w = Fade(z);

    int A = m_P[X] + Y;
    int AA = m_P[A] + Z;
    int AB = m_P[A + 1] + Z;
    int B = m_P[X + 1] + Y;
    int BA = m_P[B] + Z;
    int BB = m_P[B + 1] + Z;

    return Lerp(w,
                Lerp(v,
                     Lerp(u,
                          Grad(m_P[AA], x, y, z),
                          Grad(m_P[BA], x - 1, y, z)),
                     Lerp(u,
                          Grad(m_P[AB], x, y - 1, z),
                          Grad(m_P[BB], x - 1, y - 1, z))),
                Lerp(v,
                     Lerp(u,
                          Grad(m_P[AA + 1], x, y, z - 1),
                          Grad(m_P[BA + 1], x - 1, y, z - 1)),
                     Lerp(u, 
                          Grad(m_P[AB + 1], x, y - 1, z - 1),
                          Grad(m_P[BB + 1], x - 1, y - 1, z - 1))));
}

constexpr float PerlinNoise::Lerp(float t, float a, float b) const {
    return a + t * (b - a);
}

constexpr float PerlinNoise::Fade(float t) const {
    return t * t * t * (t * (t * 6.0 - 15.0) + 10.0);
}

constexpr float PerlinNoise::Grad(int hash, float x, float y, float z) const {
    int h = hash & 15;
    float u = h < 8 ? x : y;
    float v = h < 4 ? y : h == 12 || h == 14 ? x : z;

    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}
