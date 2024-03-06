#include "DataParametersSaveManager.h"

#include <iostream>
#include <fstream>

void DataParametersSaveManager::SaveParametersToFile(const std::string& filename, TerrainParameters* params, unsigned char* seedArray)
{
    std::ofstream file(filename);
    if (!file)
    {
        std::cout << "Nie mozna otworzyc pliku do zapisu." << std::endl;
        return;
    }

    // Zapis zmiennych do pliku
    file << "Skala: " << params->scale << std::endl;
    file << "Persistencja: " << params->persistence << std::endl;
    file << "Oktawy: " << params->octaves << std::endl;
    file << "Wysokosc: " << params->height << std::endl;
    file << "Lacunarity: " << params->lacunarity << std::endl;
    file << "Potegowanie: " << params->exponentiation << std::endl;

    // Zapis tablicy do pliku
    file << "TablicaZiarna:" << std::endl;
    for (int i = 0; i < 256; ++i)
    {
        file << static_cast<int>(seedArray[i]) << " ";
    }
    file << std::endl;

    file.close();
    std::cout << "Zapisano dane do pliku: " << filename << std::endl;
}

bool DataParametersSaveManager::LoadParametersFromFile(const std::string& filename, TerrainParameters* params, unsigned char* seedArray)
{
    std::ifstream file(filename);
    if (!file)
    {
        std::cout << "Nie mo¿na otworzyæ pliku do odczytu." << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line))
    {
        if (line.find("Skala:") != std::string::npos)
        {
            params->scale = std::stof(line.substr(line.find(":") + 1));
        }
        else if (line.find("Persistencja:") != std::string::npos)
        {
            params->persistence = std::stof(line.substr(line.find(":") + 1));
        }
        else if (line.find("Oktawy:") != std::string::npos)
        {
            params->octaves = std::stoi(line.substr(line.find(":") + 1));
        }
        else if (line.find("Wysokosc:") != std::string::npos)
        {
            params->height = std::stoi(line.substr(line.find(":") + 1));
        }
        else if (line.find("Lacunarity:") != std::string::npos)
        {
            params->lacunarity = std::stof(line.substr(line.find(":") + 1));
        }
        else if (line.find("Potegowanie:") != std::string::npos)
        {
            params->exponentiation = std::stof(line.substr(line.find(":") + 1));
        }
        else if (line.find("TablicaZiarna:") != std::string::npos)
        {
            int num;
            int i = 0;
            while (file >> num && i < 256)
            {
                //std::cout << liczba << ", ";
                if (num > 255)
                    num = 255;
                else if (num < 0)
                    num = 0;

                seedArray[i] = num;
                i++;
            }
        }
    }

    file.close();
    std::cout << "Odczytano dane z pliku: " << filename << std::endl;
    return true;
}
