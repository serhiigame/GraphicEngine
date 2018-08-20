# pragma once
#define _USE_MATH_DEFINES
#include <math.h>

#include <Mathematics/GteVector2.h>
#include <Mathematics/GteVector3.h>
#include <Mathematics/GteQuaternion.h>
#include <Mathematics/GteMatrix4x4.h>
#include <Mathematics/GteProjection.h>
#include <Graphics/GteTransform.h>



namespace engine
{
	typedef gte::Vector2<float> Vec2f;
	typedef gte::Vector3<float> Vec3f;
	typedef gte::Vector4<float> Vec4f;
	typedef gte::Matrix4x4<float> Mat4f;
	typedef gte::Quaternion<float> Quatf;
	typedef gte::Transform Transform;


	inline Mat4f MakeProjectionMatrix(float near, float far, float fov, float aspect)
	{
		Mat4f ret;
		ret.MakeIdentity();
		// set the basic projection matrix
		float scale = 1 / tan(fov * 0.5 * M_PI / 180);

		//ret(0, 0) = 1;// scale; // scale the x coordinates of the projected point 
		//ret(1, 1) = 1;// scale; // scale the y coordinates of the projected point 
		//ret(2, 2) = 1;
		//ret(2, 3) = -1.0; // set w = -z 
		//ret(3, 3) = 1;


		ret(0, 0) =  scale; // scale the x coordinates of the projected point 
		ret(1, 1) =  scale * aspect; // scale the y coordinates of the projected point 
		ret(2,2) = -far / (far - near); // used to remap z to [0,1] 
		ret(3,2) = -far * near / (far - near); // used to remap z [0,1] 
		ret(2,3) = -1; // set w = -z 
		ret(3,3) = 0;

		return ret;
	}
}