#version 460 core

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec3 inTangent;
layout(location = 3) in vec2 inTexCoord;

uniform mat4 MVP;
uniform mat4 modelMat;
uniform mat3 normalMat;

out VsOut{
	vec3 fragPosition;
	vec3 normal;
	vec3 tangent;
	vec2 texCoord;
	mat3 TBN;
} vsOut;

void main(){
	vec4 position = MVP * vec4(inPosition, 1);
	gl_Position = position;

	vsOut.fragPosition = vec3(modelMat * vec4(inPosition, 1));
	vsOut.normal = normalMat * inNormal;
	vsOut.tangent = inTangent;
	vsOut.texCoord = inTexCoord;

	//TBN
	vec3 T = normalize(vec3(modelMat * vec4(inTangent, 0.0)));
	vec3 N = normalize(vec3(modelMat * vec4(inNormal, 0.0)));

	// Re-orthogonalize T with respect to N (Gram-Schmidt process)
	T = normalize(T - dot(T, N) * N);
	vec3 B = cross(N, T);

	vsOut.TBN = mat3(T, B, N);
}