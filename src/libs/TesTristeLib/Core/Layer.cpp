#include "Layer.hpp"

namespace TesTriste {

Layer::Layer(Size size)
  : m_layerSize(size) {}

void Layer::setLayerSize(Size size) { m_layerSize = size; }
}