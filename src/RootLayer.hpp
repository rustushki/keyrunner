#ifndef ROOTLAYER_HPP
#define ROOTLAYER_HPP

class RootLayer : Layer {
public:

    static RootLayer* GetInstance();

    virtual void draw(SDL_Surface* dst);

    virtual SDL_Rect getRect() const;

    virtual void update();

private:

    static RootLayer* instance;

    RootLayer();

};

#endif//ROOTLAYER_HPP
