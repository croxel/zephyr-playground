/* 2025-07-11T09:21:23.280105 */

/* ----------------------------------------------------------------------
Copyright (c) 2022-2025 Neuton.AI, Inc.
*
The source code and its binary form are being made available on the following terms:
Redistribution, use, and modification are permitted provided that the following
conditions are met:
*
1. Redistributions of source code and/or its binary form must retain the above copyright notice,
* this list of conditions (and the disclaimer) either in the body of the source code or in
* the documentation and/or other materials provided with the distribution of the binary form, as
applicable.
*
2. The name of the copyright holder may not be used to endorse or promote products derived from this
* source code or its binary form without specific prior written permission of Neuton.AI, Inc.
*
3. Disclaimer. THIS SOURCE CODE AND ITS BINARY FORM ARE PROVIDED BY THE COPYRIGHT HOLDER "AS IS".
* THE COPYRIGHT HOLDER HEREBY DISCLAIMS ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
* BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
* PARTICULAR PURPOSE. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE HELD LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; INFRINGEMENT OF INTELLECTUAL PROPERTY RIGHTS
* OF THIRD PARTIES; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
* IN ANY WAY OUT OF THE USE OF THIS SOURCE CODE OR ITS BINARY FORM, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
---------------------------------------------------------------------- */

static const neuton_weight_t MODEL_WEIGHTS[] =
{ -120, 126, 123, 123, 28, 34, -37, 91, -128, 64, -35, -19, -94, -25, 116,
 -19, 115, -85, 57, 17, 64, -13, -36, -6, -105, 22, 125, 14, 46, -81, 125,
 -28, 105, -105, -83, -88, 108, -73, 79, -116, -98, -26, 75, 72, 108, 86,
 82, -47, 86, -98, -86, -19, 64, 105, 96, 126, -128, 18, -95, 51, -126,
 106, 24, 66, -126, -4, 74, 22, -126, 40, 123, 21, -123, 65, -124, 84, -64,
 -125, 126, -58, -42, -89, -17, 70, 54, 86, -126, -50, -114, -111, 4, -97,
 96, 30, 103, 125, -108, 124, 123, -62, -126, 29, 82, -128, 26, -126, 76,
 125, -128, -117 };

static const neuton_sources_num_t MODEL_LINKS[] =
{ 1, 2, 3, 4, 6, 7, 16, 66, 0, 66, 3, 11, 14, 15, 18, 20, 21, 25, 26, 29,
 30, 31, 32, 33, 36, 43, 51, 55, 63, 66, 2, 66, 0, 2, 3, 7, 19, 28, 29, 35,
 37, 41, 44, 45, 46, 49, 51, 52, 53, 54, 57, 58, 60, 63, 64, 66, 4, 66, 0,
 12, 38, 46, 48, 66, 1, 5, 10, 13, 16, 21, 23, 31, 34, 43, 50, 56, 61, 66,
 7, 66, 5, 8, 9, 17, 24, 25, 27, 42, 43, 46, 47, 59, 61, 62, 66, 9, 66, 18,
 22, 39, 40, 65, 66, 11, 66, 46, 66, 6, 13, 66 };

static const neuton_weights_num_t MODEL_INTERNAL_LINKS_BOUNDARIES[] =
{ 0, 9, 10, 31, 32, 57, 58, 64, 79, 80, 96, 97, 104, 105, 109 };

static const neuton_weights_num_t MODEL_EXTERNAL_LINKS_BOUNDARIES[] =
{ 8, 10, 30, 32, 56, 58, 64, 78, 80, 95, 97, 103, 105, 107, 110 };

static const neuton_coeff_t MODEL_ACTIVATION_COEFFS[] =
{ 0, 160, 0, 160, 0, 158, 0, 0, 160, 0, 160, 0, 124, 120, 158 };

static const neuton_u8_t MODEL_ACTIVATION_TYPE_MASK[] =
{ 0xd5, 0x2a };

static const neuton_neurons_num_t MODEL_OUTPUT_NEURONS[] =
{ 1, 3, 5, 14, 8, 10, 12 };
