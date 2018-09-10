#version 330 core
out vec4 color;

// in's from vertex shader
in vec3 fragment_position;  
in vec3 norm;  
in vec3 diffuse_light;
  
// lighting uniforms
uniform vec3 lamp_pos; 
uniform vec3 camera_pos;
uniform vec3 light_color;
uniform vec3 box_color;

// ambient lighting - calculated in the main program
uniform vec3 ambient_light;

void main()
{
    // Specular lighting
    float specular_strength = 0.5f;
	vec3 light_vector = normalize(lamp_pos - fragment_position);
    vec3 reflected_vector = reflect(light_vector, norm);  
    vec3 view_vector = normalize(camera_pos - fragment_position);
    float specular_value = specular_strength * pow(max(dot(view_vector, reflected_vector), 0.0), 16);
    vec3 specular_light = specular_value * light_color;  
        
    color = vec4((ambient_light + diffuse_light + specular_light) * box_color, 1.0f);
} 
