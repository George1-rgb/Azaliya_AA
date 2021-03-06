struct materialProperty
{
    vec3 diffuseColor;
    vec3 ambientColor;
    vec3 specularColor;
    float shines;
};

uniform sampler2D u_diffuseMap;
uniform sampler2D u_normalMap;
uniform sampler2D u_shadowMap;

uniform highp float u_lightPower;
uniform materialProperty u_materialProperty;
uniform bool u_isUsingDiffuseMap;
uniform bool u_isUsingNormalMap;
varying highp vec4 v_position;
varying highp vec2 v_texcoord;
varying highp vec3 v_normal;
varying highp mat3 v_tbnMatrix;
varying highp vec4 v_lightDirection;
varying highp vec4 v_positionLightMatrix;

float SampleShadowMap(sampler2D map, vec2 coords, float compare)
{
    vec4 v = texture2D(map, coords);
    float value = v.x * 255.0 + (v.y * 255.0 + (v.z * 255.0 + v.w) / 255.0) / 255.0;
    return step(compare, value);
}

float LinearInterpolation(sampler2D map, vec2 coords, float compare, vec2 texelSize)
{
    vec2 pixelPos = coords / texelSize + 0.5;
    vec2 fractPart = fract(pixelPos);
    vec2 startTexel = (pixelPos - fractPart) * texelSize;

    float blTexel = SampleShadowMap(map, startTexel, compare);
    float brTexel = SampleShadowMap(map, startTexel + vec2(texelSize.x, 0.0), compare);
    float tlTexel = SampleShadowMap(map, startTexel + vec2(0.0, texelSize.y), compare);
    float trTexel = SampleShadowMap(map, startTexel + texelSize, compare);

    float mixA = mix(blTexel, tlTexel, fractPart.y);
    float mixB = mix(brTexel, trTexel, fractPart.y);

    return mix(mixA, mixB, fractPart.x);
}

float ShadowMapPCF(sampler2D map, vec2 coords, float compare, vec2 texelSize)
{
    float result = 0.0;
    for (float y = -4.0; y < 4.0; y += 1.0)
        for (float x = -4.0; x < 4.0; x += 1.0)
        {
            vec2 offset = vec2(x, y) * texelSize;
            result += LinearInterpolation(map, coords + offset, compare, texelSize);
        }
    return result / 81.0;
}

float CalcShadowAmount(sampler2D map, vec4 shadowCoordsInit)
{
    vec3 tmp = v_positionLightMatrix.xyz / v_positionLightMatrix.w;
    tmp = tmp * vec3(0.5) + vec3(0.5);
    float offset = 10.0;
    offset *= dot(v_normal, v_lightDirection.xyz);

    return ShadowMapPCF(u_shadowMap, tmp.xy, tmp.z * 255.0 + offset, vec2(1.0 / 1024));
}

void main(void)
{

    highp float shadowCoef = CalcShadowAmount(u_shadowMap, v_positionLightMatrix);

    vec4 resultColor = vec4(0.0, 0.0, 0.0, 0.0);
    vec4 eyePosition = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 diffMatColor = texture2D(u_diffuseMap, v_texcoord);

    vec3 usingNormal = v_normal;
    if (u_isUsingNormalMap) usingNormal = normalize(texture2D(u_normalMap, v_texcoord).rgb * 2.0 - 1.0);

    vec3 eyeVect = normalize(v_position.xyz - eyePosition.xyz);
    if (u_isUsingNormalMap) eyeVect = normalize(v_tbnMatrix * eyeVect);

    vec3 lightVect = normalize(v_lightDirection.xyz);
     if (u_isUsingNormalMap) lightVect = normalize(v_tbnMatrix * lightVect);

    vec3 reflectLight = normalize(reflect(lightVect, usingNormal));
    float len = length(v_position.xyz - eyePosition.xyz);
    float specularFactor = u_materialProperty.shines;
    float ambientFactor = 0.1;

    if (u_isUsingDiffuseMap == false) diffMatColor = vec4(u_materialProperty.diffuseColor, 1.0);
    vec4 diffColor = diffMatColor * u_lightPower * max(0.0, dot(usingNormal, -lightVect));
    resultColor += diffColor;
    vec4 ambientColor = ambientFactor * diffMatColor;
    resultColor += ambientColor * vec4(u_materialProperty.ambientColor, 1.0);

    vec4 specularColor = vec4(1.0, 1.0, 1.0, 1.0) * u_lightPower * pow(max(0.0, dot(reflectLight, -eyeVect)), specularFactor);
    resultColor += specularColor * vec4(u_materialProperty.specularColor, 1.0);

    shadowCoef += 0.15;
    if(shadowCoef > 1.0) shadowCoef = 1.0;

    gl_FragColor = resultColor * shadowCoef;

}
