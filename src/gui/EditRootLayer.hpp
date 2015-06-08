#ifndef EDITROOTLAYER_HPP
#define EDITROOTLAYER_HPP

#include "RootLayer.hpp"

class EditRootLayer : public RootLayer {
public:

    static EditRootLayer* GetInstance();

private:

    static EditRootLayer* instance;

    EditRootLayer();

};

#endif//EDITROOTLAYER_HPP
