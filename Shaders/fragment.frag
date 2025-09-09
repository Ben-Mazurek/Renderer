#version 330 core

out vec3 color;
uniform vec2 iResolution;
uniform float iTime;

uniform vec3 eye = vec3(0,0,5);
uniform mat3 viewDir = mat3(
    1,0,0,
    0,1,0,
    0,0,1);


struct MatStruct{   //Vec4s used because of padding when passing from cpu. w component will be zero, just use vec.xyz and pretend its a vec3
    vec4 AmbRef;
    vec4 diffMat;
    vec4 specMat;
    vec4 AmbLight;
    float shiny;
};
layout(std140) uniform PhongMats {       //layout(std140) makes the memory structure the same as a C++ struct, but not with vec3s for some reason >:(
    MatStruct mats[2];
}objects;

const float MAX_STEPS = 1000;
const float NEAR_CLIP = 0.0;
const float FAR_CLIP = 100.0;
const float EPSILON = 0.0001;

vec3 backgroundCol = vec3(0.0, 0.0, 0.0);

float sphereSDF(vec3 p){
	return length(p) - 1.0;
}
float sdPlane( vec3 p, vec3 n, float h )
{
  // n must be normalized
  return dot(p,n) + h;
}

vec3 skybox(vec3 v)
{
    vec3 bit = 1.0 -(vec3(-v.y,-v.y,-v.y) + clamp(sign(v.y), 0., 1.));
    return (1.0 - vec3(-v.y)) * vec3(0.098, 0.443, 0.561) + vec3(-v.y);
}


float sceneSDF(vec3 p){
    //return sphereSDF(p);
	return min(sphereSDF(p), sdPlane(p, vec3(0, -1, 0), 1.5));
}

vec3 rayDirection(float fov, vec2 res, vec2 fragCoord){
	
	// res/2 gives the fragcoord of the center of the screen. Subtracting finds the position of the current coord relative to the center
	vec2 xy = fragCoord - res / 2.0;
	//fov is divided by 2 to get angle from the center. Gets tangent and divides by opposite side to get adj, or the dist to the point
	//Try drawing a triangle if it doesn't make sense
	float z = res.y / tan(radians(fov) / 2.0);

	return vec3(normalize(vec3(xy, -z))); //It's convention for the camera to point towards negative z
}

vec3 estimateNormal(vec3 p) {
    return normalize(vec3(
        sceneSDF(vec3(p.x + EPSILON, p.y, p.z)) - sceneSDF(vec3(p.x - EPSILON, p.y, p.z)),
        sceneSDF(vec3(p.x, p.y + EPSILON, p.z)) - sceneSDF(vec3(p.x, p.y - EPSILON, p.z)),
        sceneSDF(vec3(p.x, p.y, p.z  + EPSILON)) - sceneSDF(vec3(p.x, p.y, p.z - EPSILON))
    ));
}

mat3x3 xRot(float t){
    return mat3x3(
        1, 0, 0,
        0, cos(t), -sin(t),
        0, sin(t), cos(t)
    );
}
mat3x3 yRot(float t){
    return mat3x3(
        cos(t), 0, sin(t),
        0, 1, 0,
        -sin(t), 0, cos(t)
    );
}
mat3x3 zRot(float t){
    return mat3x3(
        cos(t), -sin(t), 0,
        sin(t), cos(t), 0,
        0, 0, 1
    );
}

mat3x3 xyRot(float x, float y){
    return mat3x3(
        cos(y), cos(y) * sin(x), sin(y) * cos(x),
        0, cos(x), -sin(x),
        -sin(y), cos(y) * sin(x), cos(y) * cos(x)
    );
}

vec3 phongPointLight(vec3 point, vec3 eye, vec3 lightPos, vec3 diffuseMat, vec3 diffuseLight, vec3 specularMat, 
                     float shininess, vec3 specularLight)
{
    vec3 lightDir = normalize(lightPos - point);
    vec3 normal = estimateNormal(point);
    float LdotN = dot(lightDir, normal);
    vec3 reflectedDir = 2. * LdotN * normal - lightDir;
    vec3 eyeDir = normalize(eye - point);
    vec3 diffuseResult = diffuseMat * LdotN * diffuseLight;
    vec3 specularResult = specularMat * pow(dot(reflectedDir, eyeDir), shininess) * specularLight;
    
    if(dot(reflectedDir, eyeDir) > 0.){
        return diffuseResult + specularResult;
    }
    return diffuseResult;
    
}

vec3 phongObject(vec3 point, vec3 eye, vec3 ambientReflection, vec3 specularMat, vec3 ambientLight, 
                 vec3 diffuseMat, float shininess)
{
    vec3 result = ambientReflection * ambientLight;
    
    vec3 lightPos = vec3(3., sin(iTime), 4.);
    //vec3 lightPos = vec3(1000., -1000, 0);
    
    vec3 diffuseLight = vec3(1.000,1.000,1.000);
    vec3 specularLight = vec3(0.525,0.525,0.52);
    result += phongPointLight(point, eye, lightPos, diffuseMat, diffuseLight, specularMat, shininess, specularLight); 
        
    return result;
}

float distanceEstimator(vec3 origin, vec3 dir){
	float depth = NEAR_CLIP;
	for(int i; i< MAX_STEPS; i++){
		float dist = sceneSDF(origin + depth*dir);
		if(dist < EPSILON){
			return depth;
		}
		depth += dist;
		if(depth >= FAR_CLIP){
			return FAR_CLIP;
		}
	}
	return FAR_CLIP;
}

void main(){

	color = vec3(1.0, 1.0, 1.0);
	vec3 dir = rayDirection(90.0, iResolution, gl_FragCoord.xy);
    dir *= viewDir;

	float dist = distanceEstimator(eye, dir);
	
	if(dist > FAR_CLIP - EPSILON){
		color = backgroundCol;
        color = skybox(dir);
	}
	else{
        vec3 phong = phongObject(eye + dist * dir, eye, 
            objects.mats[0].AmbRef.xyz,
            objects.mats[0].specMat.xyz, 
            objects.mats[0].AmbLight.xyz, 
            objects.mats[0].diffMat.xyz, 
            objects.mats[0].shiny);
        color = phong;

        //color = estimateNormal(eye+dist * dir);
        //color = sqrt(color);
	}


}