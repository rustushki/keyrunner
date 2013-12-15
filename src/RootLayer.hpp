#ifndef ROOTLAYER_HPP
#define ROOTLAYER_HPP

class InfoBarLayer;

class RootLayer : public Layer {
public:

    virtual void draw(SDL_Surface* dst);

    virtual SDL_Rect getRect() const;

    virtual void update();

protected:
    RootLayer(InfoBarLayer* ibl);

private:

    InfoBarLayer* ibl;

};

#endif//ROOTLAYER_HPP
