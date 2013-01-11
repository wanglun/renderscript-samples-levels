/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#include <string.h>
#include <jni.h>
#include <math.h>

/* This is a trivial JNI example where we use a native method
 * to return a new VM String. See the corresponding Java source
 * file located at:
 *
 *   apps/samples/hello-jni/project/src/com/example/hellojni/HelloJni.java
 */
void
Java_com_android_rs_levels_LevelsJniActivity_jniFilter( JNIEnv* env,
                                                  jobject thiz, jintArray InPixels, jintArray OutPixels, jfloatArray mm, jfloat mGamma, jfloat mInBlack, jfloat mOverInWMinInB, jfloat mOutWMinOutB, jfloat mOutBlack)
{
    jint *mInPixels, *mOutPixels;
    mInPixels = (*env)->GetIntArrayElements(env, InPixels, NULL);
    jint len = (*env)->GetArrayLength(env, InPixels);
    mOutPixels = (*env)->GetIntArrayElements(env, OutPixels, NULL);
    jfloat *m;
    m = (*env)->GetFloatArrayElements(env, mm, NULL);

    jint i = 0;

    for (; i < len; i++) {
	float r = (float)(mInPixels[i] & 0xff);
	float g = (float)((mInPixels[i] >> 8) & 0xff);
	float b = (float)((mInPixels[i] >> 16) & 0xff);

	float tr = r * m[0] + g * m[3] + b * m[6];
	float tg = r * m[1] + g * m[4] + b * m[7];
	float tb = r * m[2] + g * m[5] + b * m[8];
	r = tr;
	g = tg;
	b = tb;

	if (r < 0.f) r = 0.f;
	if (r > 255.f) r = 255.f;
	if (g < 0.f) g = 0.f;
	if (g > 255.f) g = 255.f;
	if (b < 0.f) b = 0.f;
	if (b > 255.f) b = 255.f;

	r = (r - mInBlack) * mOverInWMinInB;
	g = (g - mInBlack) * mOverInWMinInB;
	b = (b - mInBlack) * mOverInWMinInB;

	if (mGamma != 1.0f) {
	    r = (float)pow(r, mGamma);
	    g = (float)pow(g, mGamma);
	    b = (float)pow(b, mGamma);
	}

	r = (r * mOutWMinOutB) + mOutBlack;
	g = (g * mOutWMinOutB) + mOutBlack;
	b = (b * mOutWMinOutB) + mOutBlack;

	if (r < 0.f) r = 0.f;
	if (r > 255.f) r = 255.f;
	if (g < 0.f) g = 0.f;
	if (g > 255.f) g = 255.f;
	if (b < 0.f) b = 0.f;
	if (b > 255.f) b = 255.f;

	mOutPixels[i] = ((int)r) + (((int)g) << 8) + (((int)b) << 16)
			+ (mInPixels[i] & 0xff000000);
    }
    (*env)->ReleaseIntArrayElements(env, InPixels, mInPixels, 0);
    (*env)->ReleaseIntArrayElements(env, OutPixels, mOutPixels, 0);
}
