#version 330 core

//TODO: P1bTask5 - Modify shader to use position, normal and light positions to compute lighting.

// Output color
in vec3 fragPos;
in vec3 fragNormal;

out vec4 color;

// Object properties
uniform vec4 objColor;
uniform vec3 objPosition;
uniform float attenuationCoefficient;
uniform vec3 diffusem;
uniform vec3 specularm;
uniform float shininess;

// Light properties
uniform vec3 position1;
uniform vec3 intensityl1;
uniform vec3 diffusel1;
uniform vec3 specularl1;

uniform vec3 position2;
uniform vec3 intensityl2;
uniform vec3 diffusel2;
uniform vec3 specularl2;

// View properties
uniform vec3 viewPosition;

struct Light {
    vec3 position;
    vec3 intensity;
    vec3 diffuse;
    vec3 specular;
};


void main() {

     // Fill the struct naively for now
     // passing structs is too hard for now
    Light lights[2];
    lights[0] = Light(position1, intensityl1, diffusel1, specularl1);
    lights[1] = Light(position2, intensityl2, diffusel2, specularl2);
    
    vec3 norm = normalize(fragNormal);
    vec3 viewDir = normalize(viewPosition - fragPos);
    vec3 ambient = 0.3 * objColor.rgb;
    vec3 diffuse = vec3(0.0);
    vec3 specular = vec3(0.0);
    
    for(int i = 0; i < 2; i++) {
        float dist = length(lights[i].position - fragPos);
        vec3 lightDir = normalize(lights[i].position - fragPos);
        
        float attenuation = 1.0 / (1.0 + 0.01 * dist + 0.001 * dist * dist);
        
        // Diffuse lighting
        float diff = max(dot(norm, lightDir), 0.0);
        diffuse += attenuation * (diff * lights[i].diffuse * diffusem);

        // Specular lighting
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
        specular += attenuation * (spec * lights[i].specular * specularm);
    }
    
    // Combine all lighting components
    vec3 result = ambient + diffuse + specular;
    
    // Final output color
    color = vec4(result, objColor.a);
}

