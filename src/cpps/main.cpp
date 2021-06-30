#include "../includes/widget.h"

#include <QApplication>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

int main(int argc, char *argv[])
{
    Assimp::Importer import;
    const aiScene* scene =  import.ReadFile("123", aiProcess_Triangulate);
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
           qDebug() << "ERROR::ASSIMP::"<<import.GetErrorString();
        }

    QApplication a(argc, argv);
    Widget w;
    w.show();
    return a.exec();
}
