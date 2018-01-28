#ifndef JSON_LEVEL_MANAGER_HPP
#define JSON_LEVEL_MANAGER_HPP


#include "json.hpp"
#include "../model/LevelManager.hpp"
#include "../model/BoardModel.hpp"

using nlohmann::json;

class JsonLevelManager : public LevelManager {
public:
    explicit JsonLevelManager() = default;
    uint8_t getLevelCount() const override;
    bool levelExists(uint8_t levelNumber) const override;
    void read(BoardModel* boardModel) override;
    void write(const BoardModel* boardModel) const override;
    void reset(BoardModel* boardModel) override;

private:
    std::string getPath(uint8_t levelNumber, bool inCurrentWorkingDirectory) const;
    void toJson(json& j, const BoardModel* boardModel) const;
    void toJson(json& j, const TileCoordinate* tileCoordinate) const;
    void fromJson(const json& j, BoardModel* boardModel) const;
    void fromJson(const json& j, TileCoordinate* tileCoordinate) const;

};

#endif
