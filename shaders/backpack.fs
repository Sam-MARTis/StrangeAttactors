#version 460 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;


layout (std140, binding=0) uniform CameraInformation{
   mat4 projection;
   mat4 view;
   vec3 cameraPos;
   vec3 cameraFront;
};

// uniform sampler2D texture;
// uniform vec3 cameraPos;

// struct Material{
uniform sampler2D texture_diffuse;
uniform sampler2D texture_specular;
uniform float shineFactor;
// };
struct PointLight{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    vec3 attenuationValues;
};
struct Spotlight{
    vec3 position;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec2 cutoffs;


    vec3 attenuationValues;
};

// uniform Material material;
uniform PointLight light;
uniform Spotlight torch;

vec3 calcSpotLight(Spotlight aLight, vec3 fragPos, vec3 norm, vec3 viewDir, vec3 material_diff, vec3 material_spec, float material_shine);
vec3 calcPointLight(PointLight aLight, vec3 fragPos, vec3 norm, vec3 viewDir, vec3 material_diff, vec3 material_spec, float material_shine);
void main(){
    // float mix_value = gl_position
    // float ImgParam = (TexCoord.x - 0.5f) * cos(SimParam) + (TexCoord.y - 0.5f) *sin(SimParam);
    // FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), clamp(8*ImgParam + 0.5f, 0.f, 1.0f));
    vec3 material_diff = vec3(texture(texture_diffuse, TexCoord).rgb);
    vec3 material_spec = vec3(texture(texture_specular, TexCoord).rgb);
    vec3 viewDir = normalize(cameraPos - FragPos);


    // vec3 ambient = light.ambient*material_diff;
    
    vec3 norm = normalize(Normal);
    
    // vec3 lightDir = normalize(light.position - FragPos);
    // float lightDistance = length(light.position - FragPos);
    // float attenuation = 1.0f/(light.attenuationValues[0] + light.attenuationValues[1]*lightDistance + light.attenuationValues[2]*lightDistance*lightDistance);

    // float diff = max(dot(norm, lightDir), 0.0f);
    // vec3 diffuse = diff* light.diffuse * material_diff;

    // vec3 reflectDir = reflect(-lightDir, norm);
    // float spec = pow(max(dot(viewDir, reflectDir), 0.0f), shineFactor);
    // vec3 specular = light.specular * spec * material_spec;
    
    // vec3 result = (ambient + diffuse + specular)*attenuation;
    // light;
    vec3 pointLightContribution = calcPointLight(light, FragPos,norm, viewDir, material_diff, material_spec, shineFactor)*1.0f;
    vec3 spotLightContribution = calcSpotLight(torch, FragPos,norm, viewDir, material_diff, material_spec, shineFactor)*0.0f;

    FragColor = vec4(spotLightContribution+pointLightContribution, 1.0f);
    // FragColor = vec4(result, 1.0f);

}

vec3 calcPointLight(PointLight bLight, vec3 fragPos, vec3 norm, vec3 viewDir, vec3 material_diff, vec3 material_spec, float material_shine){
    vec3 lightDirNorm = normalize(bLight.position - fragPos);
    float blightDistance = length(bLight.position - fragPos);
    float attenuation = 1.0f/(bLight.attenuationValues[0] + bLight.attenuationValues[1]*blightDistance + bLight.attenuationValues[2]*blightDistance*blightDistance);
    
    vec3 ambient = bLight.ambient*material_diff;

    float diff = max(dot(norm, lightDirNorm), 0.0f);
    vec3 diffuse = diff*bLight.diffuse * material_diff;

    vec3 reflectDir = reflect(-lightDirNorm, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material_shine);
    vec3 specular = bLight.specular * spec * material_spec;

    return (ambient +diffuse + specular) * attenuation;
}

vec3 calcSpotLight(Spotlight aLight, vec3 fragPos, vec3 norm, vec3 viewDir, vec3 material_diff, vec3 material_spec, float material_shine){
    vec3 lightDirNorm = normalize(cameraPos - fragPos);
    float lightDistance = length(cameraPos - fragPos);
    float attenuation = 1.0f/(aLight.attenuationValues[0] + aLight.attenuationValues[1]*lightDistance + aLight.attenuationValues[2]*lightDistance*lightDistance);

    vec3 ambient = aLight.ambient*material_diff;

    float diff = max(dot(norm, lightDirNorm), 0.0f);
    vec3 diffuse = diff * aLight.diffuse * material_diff;

    
    vec3 reflectDir = reflect(-lightDirNorm, norm);
    float spec = pow(max(dot(reflectDir, viewDir),0.0f), material_shine);
    vec3 specular = aLight.specular * spec *material_spec;

    float theta = clamp(dot(normalize(-aLight.direction), lightDirNorm), 0.0f, 1.0f);
    float intensity = (theta - aLight.cutoffs.y)/(aLight.cutoffs.x - aLight.cutoffs.y);
    // float intensity = 1.0f;
    return clamp((ambient +diffuse + specular) * attenuation * intensity, 0.0f, 1.0f);

}