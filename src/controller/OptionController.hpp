#ifndef OPTION_CONTROLLER_HPP
#define OPTION_CONTROLLER_HPP

#include "../controller/Controller.hpp"
#include "../model/OptionModel.hpp"

class OptionController : public Controller {
public:
    explicit OptionController(OptionModel* model, int argc, char** argv);
    void gameLoop() final;
    OptionModel* getModel() const override;

protected:
    void updateModel(long frameDuration) final;
    bool isStillExecuting() const final;

private:
    OptionModel* optionModel;
    int argc;
    char **argv;

    void showHelp();
    void showVersion();
    bool isPositiveInt(char* str);
    void die(std::string msg);
};

#endif
