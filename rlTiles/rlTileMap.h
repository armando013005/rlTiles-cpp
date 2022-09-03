/**********************************************************************************************
*
*   raylibExtras * Utilities and Shared Components for Raylib
*
*   RLTiles * Tiled map rendering
*
*   LICENSE: MIT
*
*   Copyright (c) 2020 Jeffery Myers
*
*   Permission is hereby granted, free of charge, to any person obtaining a copy
*   of this software and associated documentation files (the "Software"), to deal
*   in the Software without restriction, including without limitation the rights
*   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*   copies of the Software, and to permit persons to whom the Software is
*   furnished to do so, subject to the following conditions:
*
*   The above copyright notice and this permission notice shall be included in all
*   copies or substantial portions of the Software.
*
*   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*   SOFTWARE.
*
**********************************************************************************************/

#pragma once

#include "raylib.h"
#include "raymath.h"

#include <stdint.h>
#include <string>
#include <vector>
#include <map>
#include <memory>

class RLTileSheet
{
public:
    int ID = 0;

    std::string SheetSource;
    int StartFrame = 0;

    std::vector<Rectangle> Tiles;

    inline Rectangle GetFrame(int tileID)
    {
        int index = tileID - StartFrame;
        if (index >= 0 && index < Tiles.size())
            return Tiles[index];

        return Rectangle{ 0,0,0,0 };
    }
};

enum class RLTiledMapTypes
{
    Orthographic,
    Isometric,
};

 struct RLTile
{
    bool FlipX = false;
    bool FilpY = false;
    bool FlipDiag = false;

    int16_t TileID = -1;
};


class RLTileLayer
{
public:
    int Width = 0;
    int Height = 0;
    int TileWidth = 0;
    int TileHeight = 0;

    bool IsObject = false;

    std::vector<RLTile> Tiles;

    Vector2 GetDisplayLocation(int x, int y, RLTiledMapTypes mode);

    int ID = 0;
    std::string name;
};

class RLTileObject{

public:
    int ID = 0;
    std::string Name;
    std::string Type;

    Rectangle Bounds = {};
    int Rotation = 0;

    int GidTile = -1; //A reference to a tile. (optional)
    bool Visible = true;

    std::string Template;
    //TODO: read templates

    enum class SubTypes
    {
        None,
        Ellipse,
        Point,
        Polygon,
        Polyline,
        Text,
    };

    SubTypes Subtype = SubTypes::None;

    class Property
    {
    public:
        std::string Name;
        std::string Type;
        std::string Value;

        inline int GetInt() const
        {
            if (Type != "int" || Value.empty())
                return 0;

            return atoi(Value.c_str());
        }

        inline float GetFloat() const
        {
            if (Type != "float" || Value.empty())
                return 0;

            return float(atof(Value.c_str()));
        }

        inline const char* GetString() const
        {
            return Value.c_str();
        }
    };

    std::vector<Property> Properties;

    inline const Property* GetProperty(const char* Name) const
    {
        for (const auto& prop : Properties)
        {
            if (prop.Name == Name)
                return &prop;
        }
        return nullptr;
    }

};


class RlTilePointObject : public RLTileObject{
public:
    Vector2 Point;
};

class RlTilePolygonObject : public RLTileObject
{
public:
    std::vector<Vector2> Points;
};

class RlTileTextObject : public RLTileObject
{
public:
    std::string Text;
    Color TextColor = WHITE;
    bool Wrap = false;

    int FontSize = 20;
    std::string FontFamily;
    bool Bold = false;
    bool Italic = false;
    bool Underline = false;
    bool Strikeout = false;
    bool Kerning = true;
    std::string HorizontalAlignment = "left";
    std::string VerticalAlignment = "top";
};

class RLTileObjectLayer : public RLTileLayer{
public:
    RLTileObjectLayer() { IsObject = true;}

    std::vector<RLTileObject> ObjectGroup;

};

class RLTileMap
{
public:
    std::map<int, RLTileSheet> Sheets;
    std::map<int, RLTileLayer> Layers;
    std::map<int, RLTileObjectLayer> Objects;

    RLTiledMapTypes MapType = RLTiledMapTypes::Orthographic;

    RLTile GetTile(int x, int y, int layerID);
};

bool RLReadTileMap(const std::string& filename, RLTileMap &map);
bool RLReadTileMapFromMemory(void* buffer, size_t bufferSize, RLTileMap& map);

