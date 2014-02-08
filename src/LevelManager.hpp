#ifndef LEVELMANAGER_HPP
#define LEVELMANAGER_HPP

class Level;

class LevelManager {
public:
    static uint16_t GetTotal();
    static Level* Read(uint8_t levelNum);
    static bool Write(Level* level);

private:
    static std::string GetPath(uint8_t levelNum, bool inCwd);

};

#endif//LEVELMANAGER_HPP
