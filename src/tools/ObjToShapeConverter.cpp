
#include <bullet/LinearMath/btVector3.h>
#include <bullet/LinearMath/btTransform.h>
#include <bullet/LinearMath/btQuaternion.h>
#include <bullet/btBulletCollisionCommon.h>

#include <memory>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <map>

#include <cstdlib>

#include <Debug.h>
#include <Math.hpp>
#include <StdUtil.hpp>
#include <JSON.h>

struct Indices {
	int v;
	int n;
};

std::vector<btVector3> vertices;
std::vector<btVector3> normals;
std::map<std::string, std::vector<std::vector<Indices>>> indicesMap;

void LoadOBJ(std::string fileName) {
	std::ifstream stream(fileName);
	std::string line, word, objectName;
	float ftemp1, ftemp2, ftemp3;
	int itemp1;
	btVector3 vtemp1;
	Indices ind;
	while(!stream.eof() && stream.good()) {
		line = "";
		word = "";
		GetLine(stream, line);
		std::stringstream s(line);
		s.str(line);
		s >> word;
		switch(word[0]) {
		case 'v':
			s >> ftemp1;
			s >> ftemp2;
			s >> ftemp3;
			if(word[1] == 'n') {
				normals.resize(normals.size()+1);
				normals.back() = btVector3(-ftemp1,ftemp2,-ftemp3);
			} else if(word[1] == 0) {
				vertices.resize(vertices.size()+1);
				vertices.back() = btVector3(-ftemp1,ftemp2,-ftemp3);
			}
			break;
		case 'o':
			s >> objectName;
			indicesMap[objectName];
			break;
		case 'f':
			indicesMap[objectName].push_back(std::vector<Indices>());
			while(true) {
				word = "";
				s >> word;
				if(word == "")
					break;
				ind.v = atoi(word.substr(0,word.find("/")).c_str())-1;
				ind.n = atoi(word.substr(word.rfind("/")+1).c_str())-1;
				indicesMap[objectName].back().push_back(ind);
			}
			break;
		}
	}
}

void RemoveDifferentExtensionsIfItExists(std::string extension) {
	std::vector<std::string> toRemove;
	for(auto it : indicesMap) {
		if(it.first.substr(it.first.length()-extension.length()) != extension)
			toRemove.emplace_back(it.first);
	}
	if(toRemove.size() != indicesMap.size()) {
		for(auto it : toRemove)
			indicesMap.erase(it);
	}
}

void CreateConvex(std::set<int> &vertexIds, JSON json) {
	json["type"] = "convex";
	json["transform"] <<= Math::EmptyTransform;
	json["vertices"].InitArray();
	JSON verts = json["vertices"];
	JSON temp;
	for(auto id : vertexIds) {
		btVector3 vert = vertices[id];
		verts.PushBack(temp = vert.x());
		verts.PushBack(temp = vert.y());
		verts.PushBack(temp = vert.z());
	}
}

bool TrySphere(std::set<int> &vertexIds, const std::vector<std::vector<Indices>> &indices, JSON json) {
	btVector3 origin(0,0,0);
	for(auto it : vertexIds) {
		origin += vertices[it];
	}
	origin /= 12.0f;
	float len2 = (vertices[*(vertexIds.end()--)]-origin).length2();
	for(auto _it : vertexIds) {
		auto it = vertices[_it];
		float len = (it-origin).length2();
		if(fabs(len-len2) >= 0.01) {
			return false;
		}
	}
	for(auto face : indices) {
		float dist2 = (vertices[face.front().v]-vertices[face.back().v]).length2();
		for(int i=1; i<face.size(); ++i) {
			float d2 = (vertices[face[i].v]-vertices[face[i-1].v]).length2();
			if(fabs(d2-dist2) > 0.01) {
				return false;
			}
		}
	}
	
	float radius = sqrt(len2);
	
	json["type"] = "sphere";
	json["transform"] <<= btTransform(btQuaternion(), origin);
	json["vertices"] = radius;
	
	return true;
}

bool TryBox(std::set<int> &vertexIds, const std::vector<std::vector<Indices>> &indices, JSON json) {
	return false;
}

bool TryCylinder(std::set<int> &vertexIds, std::map<int, std::set<int>> vertexIndexToFace, const std::vector<std::vector<Indices>> &indices, JSON json) {
	return false;
}

bool TryTrimesh(std::set<int> &vertexIds, std::map<int, std::set<int>> vertexIndexToFace, const std::vector<std::vector<Indices>> &indices, JSON json) {
	for(auto neighbourTriangles : vertexIndexToFace) {
		for(auto a : neighbourTriangles.second) {
			btVector3 na = normals[indices[a][0].n];
			for(auto b : neighbourTriangles.second) {
				btVector3 nb = normals[indices[b][0].n];
				float origdot = vertices[neighbourTriangles.first].dot(na) + 0.001f;
				if(a < b) {
					for(auto vb : indices[b]) {
						btVector3 p = vertices[vb.v];
						if(origdot < p.dot(na)){
							return false;
						}
					}
				}
			}
		}
	}
	
	json["type"] = "trimesh";
	json["transform"] <<= Math::EmptyTransform;
	json["vertices"].InitArray();
	JSON verts = json["vertices"];
	JSON temp;
	for(auto id : vertexIds) {
		btVector3 vert = vertices[id];
		verts.PushBack(temp = vert.x());
		verts.PushBack(temp = vert.y());
		verts.PushBack(temp = vert.z());
	}
	
	json["indices"].InitArray();
	JSON inds = json["indices"];
	JSON jval;
#define INDEX_OF(X) (jval = std::distance(vertexIds.begin(), vertexIds.find(X)))
	for(auto face : indices) {
		for(int i=2; i<face.size(); ++i) {
			inds.PushBack(INDEX_OF((face[0].v)));
			inds.PushBack(INDEX_OF((face[i-1].v)));
			inds.PushBack(INDEX_OF((face[i].v)));
		}
	}
#undef INDEX_OF
	return true;
}

void CreateJSONFrom(const std::vector<std::vector<Indices>> &indices, JSON json) {
	std::set<int> vertexIds;
	std::map<int, std::set<int>> vertexIndexToFace;
	for(int i=0; i<indices.size(); ++i) {
		for(int j=0; j<indices[i].size(); ++j) {
			auto id = indices[i][j];
			vertexIndexToFace[id.v].insert(i);
			vertexIds.insert(id.v);
		}
	}
	
	if(vertexIds.size() == 8) {
		if(TryBox(vertexIds, indices, json))
			return;
	} else if(vertexIds.size() == 16) {
		if(TryCylinder(vertexIds, vertexIndexToFace, indices, json))
			return;
	} else if(vertexIds.size() == 12) {
		if(TrySphere(vertexIds, indices, json))
			return;
	} else {
		if(TryTrimesh(vertexIds, vertexIndexToFace, indices, json))
			return;
	}
	
	CreateConvex(vertexIds, json);
}


int main(int argc, char **argv) {
	if(argc == 3) {
		LoadOBJ(argv[1]);
		RemoveDifferentExtensionsIfItExists(".shape");
		JSON json;
		json.InitObject();
		json["primitives"].InitArray();
		for(auto it : indicesMap) {
			JSON primitiveJson;
			primitiveJson.InitObject();
			CreateJSONFrom(it.second, primitiveJson);
			json["primitives"].PushBack(primitiveJson);
		}
		std::ofstream file(argv[2]);
		json.Write(file);
	} else {
		printf("\n usage:\n $: %s <input.mesh.obj> <output.mesh.json.shape>", argv[0]);
		return 1;
	}
	return 0;
}


