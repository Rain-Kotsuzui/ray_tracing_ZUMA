#include "Material.h"
#include "Parameter.h"

Material::Material(const float light, const float smooth, const float glass, const QColor color)
    : light(light),
      smooth(smooth),
      glass(glass),
      color(color) {}
Material::~Material() {}

bool Material::operator==(const Material &other) const
{
    return (this->color == other.color && (abs(this->smooth - other.smooth) < EPS) && (abs(this->glass - other.glass) < EPS));
}