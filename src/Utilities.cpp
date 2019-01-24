//
//  Utilities.cpp
//  test_101
//
//  Created by Ben Snell on 1/24/19.
//

#include "Utilities.hpp"

//--------------------------------------------------------------
glm::vec3 getPosition(vr::HmdMatrix34_t matrix) {
    
    // Reference: https://github.com/osudrl/OpenVR-Tracking-Example/blob/master/LighthouseTracking.cpp
    
    glm::vec3 position;
    
    position.x = matrix.m[0][3];
    position.y = matrix.m[1][3];
    position.z = matrix.m[2][3];
    
    return position;
}

//--------------------------------------------------------------
glm::quat getQuaternion(vr::HmdMatrix34_t matrix) {
    
    // Reference: https://github.com/osudrl/OpenVR-Tracking-Example/blob/master/LighthouseTracking.cpp
    
    glm::quat quaternion;
    
    // Should this abs() instead of fmax ()?
    quaternion.w = sqrt(fmax(0, 1 + matrix.m[0][0] + matrix.m[1][1] + matrix.m[2][2])) / 2;
    quaternion.x = sqrt(fmax(0, 1 + matrix.m[0][0] - matrix.m[1][1] - matrix.m[2][2])) / 2;
    quaternion.y = sqrt(fmax(0, 1 - matrix.m[0][0] + matrix.m[1][1] - matrix.m[2][2])) / 2;
    quaternion.z = sqrt(fmax(0, 1 - matrix.m[0][0] - matrix.m[1][1] + matrix.m[2][2])) / 2;
    quaternion.x = copysign(quaternion.x, matrix.m[2][1] - matrix.m[1][2]);
    quaternion.y = copysign(quaternion.y, matrix.m[0][2] - matrix.m[2][0]);
    quaternion.z = copysign(quaternion.z, matrix.m[1][0] - matrix.m[0][1]);
    
    return quaternion;
}

//--------------------------------------------------------------
glm::mat4 getTransformationMatrix(vr::HmdMatrix34_t matrix) {
    
    // The HmdMatrix34_t matrix returned from IVRsystem has the form m[3][4], where the 4x4 matrix would look like:
    //
    // in[0][0]     in[0][1]    in[0][2]    in[0][3]
    // in[1][0]     in[1][1]    in[1][2]    in[1][3]
    // in[2][0]     in[2][1]    in[2][2]    in[2][3]
    // 0            0           0           1
    //
    // where the position (x,y,z) is (in[0][3], in[1][3], in[2][3]) according to this resource:
    // https://github.com/osudrl/CassieVrControls/wiki/OpenVR-Quick-Start#the-position-vector
    // Thus, this matrix would be considered "row-major."
    
    
    // GLM uses column-major vectors, so some shifting around is required to convert HmdMatrix34_t
    // The last of 4 columns in glm::mat4 is the position vector (truncated to 3 values).
    // glm::vec3 position = glm::vec3(out[3]);
    //
    //  out[0][0]       out[1][0]       out[2][0]       out[3][0]
    //  out[0][1]       out[1][1]       out[2][1]       out[3][1]
    //  out[0][2]       out[1][2]       out[2][2]       out[3][2]
    //  out[0][3]       out[1][3]       out[2][3]       out[3][3]
    //
    // A good reference is here:
    // https://gist.github.com/kylemcdonald/411914969b25ce47786fd411f0ae6210
    
	// Alternate method:
	//
    //glm::mat4 out;
    //
    //out[3][0] = matrix.m[0][3];
    //out[3][1] = matrix.m[1][3];
    //out[3][2] = matrix.m[2][3];
    //
    //out[2][0] = matrix.m[0][2];
    //out[2][1] = matrix.m[1][2];
    //out[2][2] = matrix.m[2][2];
    //
    //out[1][0] = matrix.m[0][1];
    //out[1][1] = matrix.m[1][1];
    //out[1][2] = matrix.m[2][1];
    //
    //out[0][0] = matrix.m[0][0];
    //out[0][1] = matrix.m[1][0];
    //out[0][2] = matrix.m[2][0];

	glm::mat4 out(
		matrix.m[0][0], matrix.m[1][0], matrix.m[2][0], 0.0f,
		matrix.m[0][1], matrix.m[1][1], matrix.m[2][1], 0.0f,
		matrix.m[0][2], matrix.m[1][2], matrix.m[2][2], 0.0f,
		matrix.m[0][3], matrix.m[1][3], matrix.m[2][3], 1.0f
	);
    
    // Normalize?
    return out;
}

//--------------------------------------------------------------
string getETrackedPropertyErrorString(vr::TrackedPropertyError error) {

	// Do I need to write out the props here?

	switch (error) {
	case 0: return "success"; break;
	case 1: return "wrong data type"; break;
	case 2: return "wrong device class"; break;
	case 3: return "buffer too small"; break;
	case 4: return "unknown property"; break;
	case 5: return "invalid device"; break;
	case 6: return "could not contact server"; break;
	case 7: return "value not provided by device"; break;
	case 8: return "string exceeds maximum length"; break;
	case 9: return "not yet available... try again later"; break;
	case 10: return "permission denied"; break;
	case 11: return "invalid operation"; break;
	case 12: return "cannot write to wildcards"; break;
	case 13: return "IPC read failure"; break;
	default: return "unknown error"; break;
	}
}

//--------------------------------------------------------------
string getStringTrackedDeviceProperty(vr::IVRSystem *system, vr::TrackedDeviceIndex_t unDevice, vr::TrackedDeviceProperty prop, vr::TrackedPropertyError *peError) {

	uint32_t unRequiredBufferLen = system->GetStringTrackedDeviceProperty(unDevice, prop, NULL, 0, peError);
	if (unRequiredBufferLen == 0)
		return "";

	char *pchBuffer = new char[unRequiredBufferLen];
	unRequiredBufferLen = system->GetStringTrackedDeviceProperty(unDevice, prop, pchBuffer, unRequiredBufferLen, peError);
	std::string sResult = pchBuffer;
	delete[] pchBuffer;
	return sResult;
}

//--------------------------------------------------------------

//--------------------------------------------------------------

//--------------------------------------------------------------

//--------------------------------------------------------------

//--------------------------------------------------------------

//--------------------------------------------------------------