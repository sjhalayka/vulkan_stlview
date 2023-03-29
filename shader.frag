#version 450

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec4 LightPosition;
layout(location = 3) in vec3 Position;

layout(location = 0) out vec4 outColor;

vec3 LightIntensity = vec3(1.0, 0.0, 0.0);
vec3 MaterialKd = vec3(1.0, 1.0, 1.0);
vec3 MaterialKs = vec3(1.0, 0.5, 0.0);
vec3 MaterialKa = vec3(0.0, 0.025, 0.075);
float MaterialShininess = 10.0;






vec3 phongModelDiffAndSpec(bool do_specular)
{
    vec3 n = Normal;
    vec3 s = normalize(vec3(LightPosition.xyz) - Position);
    vec3 v = normalize(-Position.xyz);
    vec3 r = reflect( -s, n );
    float sDotN = max( dot(s,n), 0.0 );
    vec3 diffuse = LightIntensity * MaterialKd * sDotN;
    vec3 spec = vec3(0.0);

    if( sDotN > 0.0 )
    {
        spec.x = MaterialKs.x * pow( max( dot(r,v), 0.0 ), MaterialShininess );
        spec.y = MaterialKs.y * pow( max( dot(r,v), 0.0 ), MaterialShininess );
        spec.z = MaterialKs.z * pow( max( dot(r,v), 0.0 ), MaterialShininess );
    }

    vec3 n2 = Normal;
    vec3 s2 = normalize(vec3(-LightPosition) - Position);
    vec3 v2 = normalize(-Position.xyz);
    vec3 r2 = reflect( -s2, n2 );
    float sDotN2 = max( dot(s2,n2)*0.5f, 0.0 );
    vec3 diffuse2 = LightIntensity*0.25 * MaterialKa * sDotN2;

    float k = (1.0 - sDotN)/2.0;
    vec3 ret = diffuse + diffuse2 + MaterialKa*k;

    if(do_specular)
        ret = ret + spec;
    
    return ret;
}



void main() 
{
//    outColor = vec4(fragColor, 1);
 //   outColor = vec4(Normal, 1);


        vec3 diffAndSpec = phongModelDiffAndSpec(true);
        outColor = vec4(diffAndSpec, 1.0);
        outColor = pow( outColor, vec4(1.0 / 2.2) );







}
