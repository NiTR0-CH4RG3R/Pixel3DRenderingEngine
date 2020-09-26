#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

//Custom Math Library
#include "Math/Math.h"

//Standard Includes
#include <chrono>
#include <unordered_map>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iterator>

// A struct that holds the position and rotation of a object
struct Transform {
	Math::Vector3 position; // This is relative to the object space
	Math::Vector3 rotation;
	// rotation.x rotation around the x axis of the object in radians
	// rotation.y rotation around the y axis of the object in radians
	// rotation.z rotation around the z axis of the object in radians
};

// A struct that holds the information about a vertex of a mesh
struct Vertex {
	Math::Vector3 position;
};

// A struct that holds the information about a mesh
struct Mesh {

	Mesh() {};
	Mesh(std::string filename, Transform transform) : transform(transform) {
		LoadFromOBJFile(filename);
		CalculateNormals();
	}

	Mesh(std::string filename, Math::Vector3 position, Math::Vector3 rotation) {
		transform.position = position;
		transform.rotation = rotation;
		LoadFromOBJFile(filename);
		CalculateNormals();
	}

	Transform transform; // this holds the position and rotation of the mesh in world space
	std::vector<Vertex> vertices; // holds the vertices
	std::vector<unsigned short> indices; // holds the indices of triangles
	std::vector<Math::Vector3> normals;

	//a helper function to load a obj file
	bool LoadFromOBJFile(std::string filename) {
		std::ifstream file(filename);
		if (!file.is_open()) {
			return false;
		}

		std::string line;
		while (std::getline(file, line)) {
			std::istringstream iss(line);
			std::vector<std::string> words(std::istream_iterator<std::string>{iss},
				std::istream_iterator<std::string>());
			if (words[0] == "v") {
				vertices.push_back({ Math::Vector3(std::stof(words[1]), std::stof(words[2]), std::stof(words[3])) });
			}
			if (words[0] == "f") {
				indices.push_back(std::stoi(words[1]) - 1);
				indices.push_back(std::stoi(words[2]) - 1);
				indices.push_back(std::stoi(words[3]) - 1);
			}

		}

		return true;
	}

	//Calculate the normals of the each triangle of the mesh
	void CalculateNormals() {
		for (std::size_t i = 0; i < indices.size(); i += 3) {
			const Vertex& v0 = vertices[indices[i]];
			const Vertex& v1 = vertices[indices[i + 1]];
			const Vertex& v2 = vertices[indices[i + 2]];

			const Math::Vector3 line1 = v1.position - v0.position;
			const Math::Vector3 line2 = v2.position - v0.position;

			Math::Vector3 normal = Math::Vec3CrossProduct(line1, line2);
			normal.Normalize();
			normals.push_back(normal);
		}
	}
};

//A Struct that holds the information about the camera
struct Camera {
	Transform transform;
	float fCameraMovingSpeed = 2.0f;
	float fCameraRotatingSpeed = 5.0f;
};

//A struct that holds the directional light data
struct DirectionalLight {
	Math::Vector3 rotation;
};

//Our renderer class which inherits from olc::PixelGameEngine class
class Pixel3DRenderingEngine : public olc::PixelGameEngine {
public:
	Pixel3DRenderingEngine() {
		sAppName = "Pixel 3D Rendering Engine";
	}

private:
	bool OnUserCreate() override {
		//Alocate the memory for the pfDepthBuffer
		pfDepthBuffer = new float[ScreenWidth() * ScreenHeight()]; // Depth buffer must be the same size as our screen buffer
		//We must delete the pfDepthBuffer in our OnDestroy() function

		// Set up the projection matrix
		ProjectionMatrix = Math::Mat4MakeProjectionMatrix((float)ScreenWidth() / (float)ScreenHeight(), 90.0f, 0.05f, 1000.0f);

		//Set the position and the transformation of the main camera to zero vectors
		mainCamera.transform.position = Math::Vector3(0.0f, 2.0f, 0.0f);
		mainCamera.transform.rotation = Math::Vector3(0.0f, 0.0f, 0.0f);

		//Set the camera moving speed to 2.0 unit per second;
		mainCamera.fCameraMovingSpeed = 2.0f;

		//Set the camera rotating spped to 5.0 radian per second
		mainCamera.fCameraRotatingSpeed = 3.0f;

		//Set the directional light rotation to -45.0f deg around x axis, -45.0f deg around y axis and finaly -45.0f deg around z axis
		directoinalLight.rotation = Math::Vector3(
			Math::fDegToRadian(45.0f),
			Math::fDegToRadian(-45.0f),
			Math::fDegToRadian(0.0f)
		);

		//Load object models
		for (const std::pair<int, std::pair<std::string, Transform>>& obj : ObjFiles) {
			Mesh gameObject(obj.second.first, obj.second.second);
			GameObjects.insert({ obj.first, gameObject });
		}

		//Now the GameObject unordered map contains the meshes we specified in the ObjFiles map

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override {

		//Clear the screen to black before drawing anything
		Clear(olc::Pixel(255, 255, 70));
		//Clear the depdth buffer alongside with the screen buffer
		ClearDepthBuffer(pfDepthBuffer);

		//Get Inputs
		if (GetKey(olc::W).bHeld) {
			//Move forward relative to the camera
			Math::Vector3 move_direction = Math::VEC3_Forward * Math::Mat3MakeRotationZXY(mainCamera.transform.rotation);
			mainCamera.transform.position += move_direction * mainCamera.fCameraMovingSpeed * fElapsedTime;
		}
		if (GetKey(olc::S).bHeld) {
			//Move backward relative to the camera
			Math::Vector3 move_direction = Math::VEC3_Forward * Math::Mat3MakeRotationZXY(mainCamera.transform.rotation);
			mainCamera.transform.position -= move_direction * mainCamera.fCameraMovingSpeed * fElapsedTime;
		}
		if (GetKey(olc::D).bHeld) {
			//Move Right relative to the camera
			Math::Vector3 move_direction = Math::VEC3_Right * Math::Mat3MakeRotationZXY(mainCamera.transform.rotation);
			mainCamera.transform.position += move_direction * mainCamera.fCameraMovingSpeed * fElapsedTime;
		}
		if (GetKey(olc::A).bHeld) {
			//Move Left relative to the camera
			Math::Vector3 move_direction = Math::VEC3_Right * Math::Mat3MakeRotationZXY(mainCamera.transform.rotation);
			mainCamera.transform.position -= move_direction * mainCamera.fCameraMovingSpeed * fElapsedTime;
		}
		if (GetKey(olc::Q).bHeld) {
			//move Up relative to the camera
			Math::Vector3 move_direction = Math::VEC3_Up * Math::Mat3MakeRotationZXY(mainCamera.transform.rotation);
			mainCamera.transform.position += move_direction * mainCamera.fCameraMovingSpeed * fElapsedTime;
		}
		if (GetKey(olc::E).bHeld) {
			//move Down relative to the camera
			Math::Vector3 move_direction = Math::VEC3_Up * Math::Mat3MakeRotationZXY(mainCamera.transform.rotation);
			mainCamera.transform.position -= move_direction * mainCamera.fCameraMovingSpeed * fElapsedTime;
		}
		if (GetKey(olc::L).bHeld) {
			//Rotate clockwise around y axis of the camera
			mainCamera.transform.rotation.y += mainCamera.fCameraRotatingSpeed * fElapsedTime;
		}
		if (GetKey(olc::J).bHeld) {
			//Rotate counter-clockwise around y axis of the camera
			mainCamera.transform.rotation.y -= mainCamera.fCameraRotatingSpeed * fElapsedTime;
		}
		if (GetKey(olc::NP8).bHeld) {
			//Rotate clockwise around x axis of the directoinalLight
			directoinalLight.rotation.x -= 1.0f * fElapsedTime;
		}
		if (GetKey(olc::NP2).bHeld) {
			//Rotate counter-clockwise around x axis of the directoinalLight
			directoinalLight.rotation.x += 1.0f * fElapsedTime;
		}
		if (GetKey(olc::NP6).bHeld) {
			//Rotate clockwise around y axis of the directoinalLight
			directoinalLight.rotation.y += 1.0f * fElapsedTime;
		}
		if (GetKey(olc::NP4).bHeld) {
			//Rotate counter-clockwise around y axis of the directoinalLight
			directoinalLight.rotation.y -= 1.0f * fElapsedTime;
		}

		////Rotate the cube around its x axis and z axis
		//GameObjects[0].transform.rotation.x += 1.0f * fElapsedTime;
		//GameObjects[0].transform.rotation.z += 1.0f * fElapsedTime;

		//Rendering routine
		//Loop through the elements of the GameObjects map
		for (const std::pair<int, Mesh>& GameObject : GameObjects) {
			//GameObject.second contains the actual mesh
			//GameObject.first conatains the id of that mesh. We don't need that id in this loop. But It is useful if we needed to make changes to the position or the rotation of that particuler object. We can simply get the mesh by using that id as the key from GameObjects map
			std::vector<Math::Vector4> vec4TransformedVertices;
			for (const Vertex& vertex : GameObject.second.vertices) {
				Math::Vector4 transformedVertex =
					Math::Vector4(vertex.position) * // vertex.position is in object space. Meaning, It's relative to the object's origin
					Math::Mat4MakeRotationZXY(GameObject.second.transform.rotation) * // We rotate the vertex.position according to the object's rotation information in object space
					Math::Mat4MakeTranslation(GameObject.second.transform.position) * // Convert the position from Object space to world space by adding the object position to the vertex position
					Math::Mat4MakeTranslationInv(mainCamera.transform.position) *
					Math::Mat4MakeRotationZXYInv(mainCamera.transform.rotation) * // Get the position of the vertex in CamaraSpcae by translating and rotating the vertex by camera position and rotation
					ProjectionMatrix; // Finaly Multiply the position of the vertex so we can convert it to the screen spcae


				//This is just a quick fix alternative to clipping. Once triangle clipping algorithm implemented this check won't be neccesory 
				if (transformedVertex.w > 1.0f) {
					// Get the position of the vertex in normalized screen space by deviding the vector by its w component
					transformedVertex /= transformedVertex.w;
					// Finaly convert it to real screen cordinates
					transformedVertex.x = (transformedVertex.x * 0.5f + 0.5f) * (float)ScreenWidth();
					transformedVertex.y = (1.0f - (transformedVertex.y * 0.5f + 0.5f)) * (float)ScreenHeight();
				}
				// Push the final transformed vertex position to vec4TransformedVertices vector
				vec4TransformedVertices.push_back(transformedVertex);
			}

			for (std::size_t i = 0; i < GameObject.second.indices.size(); i += 3) {

				//Get the right points of the right triangle from vec4TransformedVertices
				Math::Vector4 p0_in_screen_space = vec4TransformedVertices[GameObject.second.indices[i]];
				Math::Vector4 p1_in_screen_space = vec4TransformedVertices[GameObject.second.indices[i + 1]];
				Math::Vector4 p2_in_screen_space = vec4TransformedVertices[GameObject.second.indices[i + 2]];

				//This is just a quick fix alternative to clipping. Once triangle clipping algorithm implemented this check won't be neccesory 
				// TODO : Implement a triangle clipping algorithm
				if (p0_in_screen_space.w == 1.0f && p1_in_screen_space.w == 1.0f && p2_in_screen_space.w == 1.0f) {

					//Get the normal of this particular triangle (in world space)
					Math::Vector3 normal = GameObject.second.normals[(int)(i / 3)];

					//Convert the normal from Object space to world space by rotating it by object's rotation
					normal = normal * Math::Mat3MakeRotationZXY(GameObject.second.transform.rotation);
					normal.Normalize();

					//Normal of the object relative to the main camera ( This is for testing purposes only )
					Math::Vector3 normal_relative_to_mainCamera = normal * Math::Mat3MakeRotationZXYInv(mainCamera.transform.rotation);

					//Get the vertex of the triangle relative to the camera (Vector4)
					Math::Vector4 vec4_vertex_of_the_triangle_rel_to_mainCamera = Math::Vector4(GameObject.second.vertices[GameObject.second.indices[i + 1]].position) * Math::Mat4MakeRotationZXY(GameObject.second.transform.rotation) * Math::Mat4MakeTranslation(GameObject.second.transform.position) * Math::Mat4MakeTranslationInv(mainCamera.transform.position) * Math::Mat4MakeRotationZXYInv(mainCamera.transform.rotation);

					//Turn the above vertex position to vector3 So we can apply this to Math::Vec3DotProduct() function below
					Math::Vector3 vertex_of_the_triangle_rel_to_mainCamera(
						vec4_vertex_of_the_triangle_rel_to_mainCamera.x,
						vec4_vertex_of_the_triangle_rel_to_mainCamera.y,
						vec4_vertex_of_the_triangle_rel_to_mainCamera.z
					);


					//Directional Light direction
					//We could pre calcultate this instead of recalculating this every frame
					//But this way it gets a more dynamic look. I mean if you modified the rotation of the directional light at runtime pre calculated results won't give you realtime results
					Math::Vector3 directional_light_direction = Math::VEC3_Forward * Math::Mat3MakeRotationZXY(directoinalLight.rotation);
					directional_light_direction.Normalize();

					//Since we are calculating the dot product of 2 normalized vectors dot product will be between -1 and 1. We have to normalize that before feeding it into the ClassifyPixel() function to determine the color of the pixel
					float dot_product_of_the_triangle_normal_and_light_direction = Math::Vec3DotProduct(normal, directional_light_direction);

					//Normalize the dot_product_of_the_triangle_normal_and_light_direction
					dot_product_of_the_triangle_normal_and_light_direction = dot_product_of_the_triangle_normal_and_light_direction * 0.5f + 0.5f;



					float pixel_grayscaled_color = dot_product_of_the_triangle_normal_and_light_direction * 255.0f;

					//Clamp the pixel_grayscaled_color So it won't go out of boundary
					if (pixel_grayscaled_color < 0.0f) pixel_grayscaled_color = 0.0f;
					else if(pixel_grayscaled_color > 255.0f) pixel_grayscaled_color = 255.0f;
					
					olc::Pixel pixel((int)pixel_grayscaled_color, (int)pixel_grayscaled_color, (int)pixel_grayscaled_color);

					// Only draw the triangle if the dot product of the vertex_of_the_triangle_rel_to_mainCamera vector and normal_relative_to_mainCamera vector is equal or less than zero. \
					//This is for backface culling. If you want to know about why we do this. Google "How does backface culling work in computer graphics) 

					// TODO : Right now Getting the dot product of the vertex_of_the_triangle_rel_to_mainCamera and normal_relative_to_mainCamera to determine to cull a face or not doesn't work very well. 
					//So I have skipped the backface culling part for now. I may fix it later. Right now back face culling doesn't make any visual difernce because we already have implemented the depth buffer. 
					//It certenly gives a perfomance improvement so I will fix it. Until then no backface culling!

					//if (Math::Vec3DotProduct(vertex_of_the_triangle_rel_to_mainCamera, normal_relative_to_mainCamera) <= 0.0f) {
						//Finaly draw the driangle in wireframe mode using DrawTriangle() function
						//DrawTriangle( Math::Vector2i((int)p0_in_screen_space.x, (int)p0_in_screen_space.y), Math::Vector2i((int)p1_in_screen_space.x, (int)p1_in_screen_space.y), Math::Vector2i((int)p2_in_screen_space.x, (int)p2_in_screen_space.y) );

						//Finaly Rasterize the triangle
					RasterizeTriangle(
						Math::Vector3(p0_in_screen_space.x, p0_in_screen_space.y, 1.0f / p0_in_screen_space.z),
						Math::Vector3(p1_in_screen_space.x, p1_in_screen_space.y, 1.0f / p1_in_screen_space.z),
						Math::Vector3(p2_in_screen_space.x, p2_in_screen_space.y, 1.0f / p2_in_screen_space.z),
						pfDepthBuffer,
						pixel
					);
					//}
				}
			}
		}

		return true;
	}

	bool OnUserDestroy() override {
		//Delete the float array we created.
		if (pfDepthBuffer) delete[] pfDepthBuffer;
		return true;
	}

private:
	//Clear DepthBuffer
	void ClearDepthBuffer(float* _pfDepthBuffer) {
		memset(_pfDepthBuffer, 0, sizeof(float) * ScreenWidth() * ScreenHeight());
	}

	//Rasterize the triangle 
	void RasterizeTriangle(Math::Vector3 p0,
		Math::Vector3 p1, Math::Vector3 p2,
		float* _pfDepthBuffer,
		olc::Pixel p = olc::WHITE ) {

		//Sort the points according to the y values of points
		if (p0.y > p1.y) { std::swap(p0, p1); }
		if (p0.y > p2.y) { std::swap(p0, p2); }
		if (p1.y > p2.y) { std::swap(p1, p2); }
		//Now p0 is the point with the lowest y coordinate
		//p2 is the point with the highest y coordinate

		//x and y must be in integers. But for depth calculations we need z in form of floating points
		Math::Vector2i xy_values_of_p0((int)p0.x, (int)p0.y); float z_value_of_p0 = p0.z;
		Math::Vector2i xy_values_of_p1((int)p1.x, (int)p1.y); float z_value_of_p1 = p1.z;
		Math::Vector2i xy_values_of_p2((int)p2.x, (int)p2.y); float z_value_of_p2 = p2.z;

		Math::Vector2i delta_xy_of_p0p2_line = xy_values_of_p2 - xy_values_of_p0; float delta_z_value_of_p0p2_line = z_value_of_p2 - z_value_of_p0;
		Math::Vector2i delta_xy_of_p0p1_line = xy_values_of_p1 - xy_values_of_p0; float delta_z_value_of_p0p1_line = z_value_of_p1 - z_value_of_p0;

		//Draw the first half of the triangle
		if (delta_xy_of_p0p1_line.y) {
			for (int _y = 0; _y < delta_xy_of_p0p1_line.y; _y++) {
				//Get the two x values of the two lines from the _y value

				//Get the x value of p0p2 line (Relative to p0)
				Math::Vector2i xy_values_of_current_point_on_p0p2_line;
				xy_values_of_current_point_on_p0p2_line.y = _y;
				xy_values_of_current_point_on_p0p2_line.x = (int)((float)xy_values_of_current_point_on_p0p2_line.y / (float)delta_xy_of_p0p2_line.y * (float)delta_xy_of_p0p2_line.x);


				//Get the z value of (x_value_of_p0p2_line, _y) point of the line. (Relative to p0)
				float z_value_of_the_current_point_on_p0p2_line = ((float)xy_values_of_current_point_on_p0p2_line.Magnitude() / (float)delta_xy_of_p0p2_line.Magnitude()) * delta_z_value_of_p0p2_line;

				//Get the x value of p0p1 line (Relative to p0)
				Math::Vector2i xy_values_of_current_point_on_p0p1_line;
				xy_values_of_current_point_on_p0p1_line.y = _y;
				xy_values_of_current_point_on_p0p1_line.x = (int)((float)xy_values_of_current_point_on_p0p1_line.y / (float)delta_xy_of_p0p1_line.y * (float)delta_xy_of_p0p1_line.x);


				//Get the z value of (x_value_of_p0p1_line, _y) point of the line. (Relative to p0)
				float z_value_of_the_current_point_on_p0p1_line = ((float)xy_values_of_current_point_on_p0p1_line.Magnitude() / (float)delta_xy_of_p0p1_line.Magnitude()) * delta_z_value_of_p0p1_line;

				//sort the values according to the x values
				if (xy_values_of_current_point_on_p0p1_line.x > xy_values_of_current_point_on_p0p2_line.x) {
					std::swap(xy_values_of_current_point_on_p0p1_line, xy_values_of_current_point_on_p0p2_line);
					std::swap(z_value_of_the_current_point_on_p0p1_line, z_value_of_the_current_point_on_p0p2_line);
				}

				for (int _x = xy_values_of_current_point_on_p0p1_line.x; _x < xy_values_of_current_point_on_p0p2_line.x; _x++) {
					// Z value of the current pixel relative to the p0
					float z_value_of_the_current_pixel = (float)(_x - xy_values_of_current_point_on_p0p1_line.x) / (float)(xy_values_of_current_point_on_p0p2_line.x - xy_values_of_current_point_on_p0p1_line.x) * (z_value_of_the_current_point_on_p0p2_line - z_value_of_the_current_point_on_p0p1_line) + z_value_of_the_current_point_on_p0p1_line;

					//real x
					int x = _x + xy_values_of_p0.x;
					int y = _y + xy_values_of_p0.y;
					float z = z_value_of_the_current_pixel + z_value_of_p0;

					//Finaly draw the pixel if the depth value of that pixel higher than the depth value already there
					if (x >= 0 && x < ScreenWidth() && y >= 0 && y < ScreenHeight()) {
						if (z > _pfDepthBuffer[x + y * ScreenWidth()]) {
							Draw(x, y, p);
							_pfDepthBuffer[x + y * ScreenWidth()] = z;
						}
					}

				}
			}
		}

		Math::Vector2i delta_xy_of_p1p2_line = xy_values_of_p2 - xy_values_of_p1; float delta_z_value_of_p1p2_line = z_value_of_p2 - z_value_of_p1;

		//Draw the next half of the triangle
		if (delta_xy_of_p1p2_line.y) {
			for (int _y = 0; _y <= delta_xy_of_p1p2_line.y; _y++) {
				//Just like above
				//Get the two x values of the two lines from the _y value

				//Get the x value of p0p2 line (Relative to p0)
				Math::Vector2i xy_values_of_current_point_on_p0p2_line;
				xy_values_of_current_point_on_p0p2_line.y = _y + delta_xy_of_p0p1_line.y;
				xy_values_of_current_point_on_p0p2_line.x = (int)((float)xy_values_of_current_point_on_p0p2_line.y / (float)delta_xy_of_p0p2_line.y * (float)delta_xy_of_p0p2_line.x);

				//Get the z value of (x_value_of_p0p2_line, _y) point of the line. (Relative to p1)
				float z_value_of_the_current_point_on_p0p2_line = ((float)xy_values_of_current_point_on_p0p2_line.Magnitude() / (float)delta_xy_of_p0p2_line.Magnitude()) * delta_z_value_of_p0p2_line - delta_z_value_of_p0p1_line;

				//Make the x value of p0p2 line relative to p1
				xy_values_of_current_point_on_p0p2_line.x -= delta_xy_of_p0p1_line.x;

				//Get the x value of p1p2 line (Relative to p1)
				Math::Vector2i xy_values_of_current_point_on_p1p2_line;
				xy_values_of_current_point_on_p1p2_line.y = _y;
				xy_values_of_current_point_on_p1p2_line.x = (int)((float)xy_values_of_current_point_on_p1p2_line.y / (float)delta_xy_of_p1p2_line.y * (float)delta_xy_of_p1p2_line.x);

				//Get the z value of (x_value_of_p0p1_line, _y) point of the line. (Relative to p1)
				float z_value_of_the_current_point_on_p1p2_line = ((float)xy_values_of_current_point_on_p1p2_line.Magnitude() / (float)delta_xy_of_p1p2_line.Magnitude()) * delta_z_value_of_p1p2_line;

				//sort the values according to the x values
				if (xy_values_of_current_point_on_p1p2_line.x > xy_values_of_current_point_on_p0p2_line.x) {
					std::swap(xy_values_of_current_point_on_p1p2_line, xy_values_of_current_point_on_p0p2_line);
					std::swap(z_value_of_the_current_point_on_p1p2_line, z_value_of_the_current_point_on_p0p2_line);
				}


				for (int _x = xy_values_of_current_point_on_p1p2_line.x; _x < xy_values_of_current_point_on_p0p2_line.x; _x++) {
					// Z value of the current pixel relative to the p0
					float z_value_of_the_current_pixel = (float)(_x - xy_values_of_current_point_on_p1p2_line.x) / (float)(xy_values_of_current_point_on_p0p2_line.x - xy_values_of_current_point_on_p1p2_line.x) * (z_value_of_the_current_point_on_p0p2_line - z_value_of_the_current_point_on_p1p2_line) + z_value_of_the_current_point_on_p1p2_line;

					//real x
					int x = _x + xy_values_of_p1.x;
					int y = _y + xy_values_of_p1.y;
					float z = z_value_of_the_current_pixel + z_value_of_p1;

					if (x >= 0 && x < ScreenWidth() && y >= 0 && y < ScreenHeight()) {
						if (z > _pfDepthBuffer[x + y * ScreenWidth()]) {
							Draw(x, y, p);
							_pfDepthBuffer[x + y * ScreenWidth()] = z;
						}
					}

				}

			}
		}
	}



private:
	std::unordered_map<int /*object_id*/, std::pair< std::string /*Filename / Filepath*/, Transform /*position and rotation data*/>> ObjFiles = {
		//{0, { "Models/Box.obj" ,	{Math::Vector3(0.0f, 0.0f, 4.0f), Math::Vector3(0.0f, 0.0f, 0.0f)} }},
		//{1, { "Models/Monkey.obj" , {Math::Vector3(3.0f, 0.0f, 5.0f), Math::Vector3(0.0f, 0.0f, 0.0f)} }},
		//{2, { "Models/human_female.obj", {Math::Vector3(-4.0f, 0.0f, 5.0f), Math::Vector3(0.0f, 0.0f, 0.0f)}}},
	{0, { "Models/fantacy_tree_house.obj" ,	{Math::Vector3(0.0f, 0.0f, 20.0f), Math::Vector3(0.0f, 0.0f, 0.0f)} }},
	};
	std::unordered_map<int /*object_id*/, Mesh /*Actual mesh*/> GameObjects;

	//Our projection matrix
	Math::Mat4x4 ProjectionMatrix;

	//Our main camera
	Camera mainCamera;

	//Our Directional Light
	//For the time being this engine still only support one Directional Light. In future I plan on Adding support for multiple light sources and multiple light sources types (such as Point Light, Directional Light, Spot Light, etc.)
	// TODO : Add support for multiple light sources
	// TODO : Add support for multiple light source types
	DirectionalLight directoinalLight;

	//Pointer to the depth buffer where we store the information about depth values of every pixel
	float* pfDepthBuffer = nullptr;

};

int main() {
	Pixel3DRenderingEngine renderingEngine;

	if (renderingEngine.Construct(800, 600, 1, 1) == olc::rcode::OK) {
		renderingEngine.Start();
	}

	return 0;
}