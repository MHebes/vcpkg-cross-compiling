#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <assimp/Exporter.hpp>
#include <assimp/Importer.hpp>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

/// Converts any mesh file to a binary gltf file.
void ProcessFile(const fs::path &inputFilePath, const fs::path &outputFilePath)
{
  Assimp::Importer importer;
  const aiScene *scene =
      importer.ReadFile(inputFilePath.string(),
                        aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
  {
    std::cerr << "Error importing file: " << importer.GetErrorString() << std::endl;
    return;
  }

  Assimp::Exporter exporter;
  if (exporter.Export(scene, "glb2", outputFilePath.string()) != aiReturn_SUCCESS)
  {
    std::cerr << "Error exporting file: " << exporter.GetErrorString() << std::endl;
    return;
  }

  std::cout << "Successfully exported " << inputFilePath << " to " << outputFilePath << std::endl;
}

int main(int argc, char *argv[])
{
  try
  {
    if (argc != 3)
    {
      std::cerr << "Usage: " << argv[0] << " <input_file> <output_file>" << std::endl;
      return -1;
    }

    fs::path inputPath(argv[1]);
    fs::path outputPath(argv[2]);

    if (!fs::exists(outputPath.parent_path()))
    {
      fs::create_directories(outputPath.parent_path());
    }

    ProcessFile(inputPath, outputPath);

    return 0;
  }
  catch (const std::exception &e)
  {
    std::cerr << e.what();
    return 1;
  }
}
