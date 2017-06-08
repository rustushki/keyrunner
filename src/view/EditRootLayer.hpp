#ifndef EDIT_ROOT_LAYER_HPP
#define EDIT_ROOT_LAYER_HPP

#include "RootLayer.hpp"

class EditRootLayer : public RootLayer {
public:

    static EditRootLayer* GetInstance();

private:

    static EditRootLayer* instance;

    EditRootLayer();

};

#endif
