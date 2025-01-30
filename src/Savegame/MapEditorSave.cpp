/*
 * Copyright 2010-2024 OpenXcom Developers.
 *
 * This file is part of OpenXcom.
 *
 * OpenXcom is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OpenXcom is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OpenXcom.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "MapEditorSave.h"
#include <sstream>
#include <yaml-cpp/yaml.h>
#include "../Engine/CrossPlatform.h"
#include "../Engine/Exception.h"
#include "../Engine/Options.h"

namespace OpenXcom
{

const std::string MapEditorSave::AUTOSAVE_MAPEDITOR = "_auto_mapeditor_.asav",
				  MapEditorSave::MAINSAVE_MAPEDITOR = "mapeditor.sav";

const std::string MapEditorSave::MAP_DIRECTORY = "/MAPS",
				  MapEditorSave::RMP_DIRECTORY = "/ROUTES";

/**
 * Initializes the class for storing information for saving or editing maps
 */
MapEditorSave::MapEditorSave()
{
    _savedMapFiles.clear();
    _matchedFiles.clear();
}

/**
 * Deletes data on edited map files from memory
 */
MapEditorSave::~MapEditorSave()
{

}

/**
 * Loads the data on edited map files from the user directory
 */
void MapEditorSave::load()
{
    std::string filename = MAINSAVE_MAPEDITOR;
	std::string filepath = Options::getMasterUserFolder() + filename;

    if(CrossPlatform::fileExists(filepath))
    {
        YAML::YamlRootNodeReader reader(filepath, false, false);
        for (const auto& savedMapFileReader : reader["savedMapFiles"].children())
        {
            MapFileInfo mapFile; 
            savedMapFileReader.tryRead("name", mapFile.name);
            savedMapFileReader.tryRead("baseDirectory", mapFile.baseDirectory );
            savedMapFileReader.tryRead("mods", mapFile.mods);
            savedMapFileReader.tryRead("terrain", mapFile.terrain);
            savedMapFileReader.tryRead("mcds", mapFile.mcds);
            _savedMapFiles.push_back(mapFile);
        }
    }

}

/**
 * Saves the data on edited map files to the user directory
 */
void MapEditorSave::save()
{
    YAML::YamlRootNodeWriter root;
    root.setAsMap();
	YAML::YamlNodeWriter savedMapFilesSequence = root["savedMapFiles"]; // create a new child with the specified key
	savedMapFilesSequence.setAsSeq(); // set the value to be a YAML sequence
	// Saves the data on the edited maps
	for (auto mapFile : _savedMapFiles)
	{
		YAML::YamlNodeWriter savedMapFileWriter = savedMapFilesSequence.write(); // creates a new sequence element
		savedMapFileWriter.setAsMap(); // set the sequence element to be a YAML mapping container        
		savedMapFileWriter.write("name", mapFile.name);
		savedMapFileWriter.write("baseDirectory", mapFile.baseDirectory);
		savedMapFileWriter.write("mods", mapFile.mods);
		savedMapFileWriter.write("terrain", mapFile.terrain);
		savedMapFileWriter.write("mcds", mapFile.mcds);
    }

	YAML::YamlString output = root.emit();
    std::string filename = MAINSAVE_MAPEDITOR;
	std::string filepath = Options::getMasterUserFolder() + filename;
	if (!CrossPlatform::writeFile(filepath, output.yaml))
	{
		throw Exception("Failed to save " + filepath);
	}
}

/**
 * Gets the data for the map file we want to load
 * @return pointer to the map info
 */
MapFileInfo *MapEditorSave::getMapFileToLoad()
{
    return &_mapFileToLoad;
}

/**
 * Clears the MapFileInfo for the file we want to load
 */
void MapEditorSave::clearMapFileToLoad()
{
    MapFileInfo fileInfo;
    _mapFileToLoad = fileInfo;
}

/**
 * Gets the data for the current map being edited
 * @return pointer to the current map info
 */
MapFileInfo *MapEditorSave::getCurrentMapFile()
{
    return &_currentMapFile;
}

/**
 * Adds the data on a newly saved map file to the list
 * @param fileInfo information on the new edited map file
 */
void MapEditorSave::addMap(MapFileInfo fileInfo)
{
    MapFileInfo existingMap = getMapFileInfo(fileInfo.baseDirectory, fileInfo.name);

    // Only add a new entry if one doesn't already exist
    if(existingMap.name.size() == 0)
    {
        _savedMapFiles.push_back(fileInfo);
    }

    _currentMapFile = fileInfo;
}

/**
 * Search for a specific entry in the list of edited map files
 * @return copy of the data for the map file
 */
MapFileInfo MapEditorSave::getMapFileInfo(std::string mapDirectory, std::string mapName)
{
    MapFileInfo fileInfo;
    fileInfo.name = "";

    // TODO: more efficient search?
    for (size_t i = 0; i != _savedMapFiles.size(); ++i)
    {
        // TODO: break points in order: baseDirectory, name, terrain, mcds, mods
        if (mapName == _savedMapFiles.at(i).name && mapDirectory == _savedMapFiles.at(i).baseDirectory)
        {
            fileInfo = _savedMapFiles.at(i);
            break;
        }
    }

    return fileInfo;
}

/**
 * Search for entries matching the given directory + map name
 * Populates the list of matched files returned by getMatchedFiles
 * @param fileInfo pointer to the information on the map file we're looking for
 * @return number of matching entries found in the saved map file data
 */
size_t MapEditorSave::findMatchingFiles(MapFileInfo *fileInfo)
{
    _mapFileToLoad = *fileInfo;
    _matchedFiles.clear();

    if (fileInfo->baseDirectory.empty() || fileInfo->name.empty())
        return 0;

    size_t numFound = 0;

    for (auto i : _savedMapFiles)
    {
        if (i.baseDirectory == fileInfo->baseDirectory && i.name == fileInfo->name)
        {
            _matchedFiles.push_back(i);
            ++numFound;
        }
    }

    if (numFound == 1)
    {
        _mapFileToLoad = _matchedFiles.front();
        _mapFileToLoad.extension = fileInfo->extension;
    }

    return numFound;
}

/**
 * Gets the list of entries found by the search
 * @return pointer to the list of MapFileInfo
 */
std::vector<MapFileInfo> *MapEditorSave::getMatchedFiles()
{
    return &_matchedFiles;
}

}
