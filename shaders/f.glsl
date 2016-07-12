#version 330 core

out vec4 outcolor;

in vec2 tc;

uniform vec2 res;
uniform sampler2D texture0;

// effects
uniform float brightness;
uniform float tint_r;
uniform float tint_g;
uniform float tint_b;
uniform int grayscale;
uniform int inverse;
uniform int vibrance;
uniform int saturation;
uniform int desaturation;
uniform int vignette;
uniform float v_radius;

vec4 tint;

vec4 texColor;
vec4 final_color;

void greatest(in vec3 c, out float g, out int i){

    if(c.x > c.y){

        if(c.x > c.z){
            g  = c.x;
            i = 1;
        } else {
            g  = c.z;
            i = 3;
        }

    } else {
        if(c.y > c.z){
            g  = c.y;
            i = 2;
        } else {
            g  = c.z;
            i = 3;
        }
    }

}
void lowest(in vec3 c, out float l, out int i){

    if(c.x < c.y){
        if(c.x < c.z){
            l = c.x;
            i = 1;
        } else {
            l = c.z;
            i = 3;
        }
    } else {
        if(c.y < c.z){
            l = c.y;
            i = 2;
        } else {
            l = c.z;
            i = 3;
        }
    }

}

void main(){



    tint = vec4(tint_r, tint_g, tint_b, 1.0f);
    texColor = texture2D(texture0, tc);
    final_color =  tint * vec4(brightness) * texColor;

    if(grayscale > 0){
        float avg = (final_color.r + final_color.g + final_color.b)/3;
        final_color = vec4( vec3( avg ) , 1.0f);
    }

    if(inverse > 0) {
        final_color.rgb = 1- final_color.rgb;
    }

    if(vibrance > 0){
        float avg = (final_color.r + final_color.g + final_color.b)/3;

        if(final_color.r > avg+0.05 && final_color.r < 0.7) final_color.r += 0.065; else final_color.r -= 0.05;
        if(final_color.g > avg+0.05 && final_color.g < 0.7) final_color.g += 0.065; else final_color.g -= 0.05;
        if(final_color.b > avg+0.05 && final_color.b < 0.7) final_color.b += 0.065; else final_color.b -= 0.05;

    }

    if(saturation > 0){
        float avg = (final_color.r + final_color.g + final_color.b)/3;
        if(final_color.r > avg) final_color.r += 0.065;
        if(final_color.g > avg) final_color.g += 0.065;
        if(final_color.b > avg) final_color.b += 0.065;
    }

    if(desaturation > 0){
        float avg = (final_color.r + final_color.g + final_color.b)/3;
        if(final_color.r > avg) final_color.r -= 0.065;
        if(final_color.g > avg) final_color.g -= 0.065;
        if(final_color.b > avg) final_color.b -= 0.065;
    }

    if(vignette > 0){

        vec2 pp = (gl_FragCoord.xy/res) - vec2(0.5);// range -0.5 to 0.5

        float len = length(pp);// = sqrt(((0.5)*(0.5)) + ((center.y)*(center.y)));

        final_color.rgb *= smoothstep( 0.5 , 0.5-v_radius , len );

    }



	outcolor = final_color;

}


