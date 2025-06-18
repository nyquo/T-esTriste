#include "Layer.hpp"

namespace TesTriste {

Layer::Layer(float layerWidth, float layerHeight)
  : m_layerWidth(layerWidth)
  , m_layerHeight(layerHeight) {}

void Layer::setLayerSize(float layerWidth, float layerHeight) {
    m_layerWidth = layerWidth;
    m_layerHeight = layerHeight;
}
}