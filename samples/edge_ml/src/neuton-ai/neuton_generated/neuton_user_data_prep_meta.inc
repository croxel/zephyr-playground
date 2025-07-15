/* 2025-07-11T09:21:23.281544 */

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

/** Defines input(also used for LAG) features MIN scaling factor
 */
static const neuton_input_t INPUT_FEATURES_SCALE_MIN[] =
{ -19612, -19612, -19612, -12870, -5785, -8455 };

/** Defines input(also used for LAG) features MAX scaling factor
 */
static const neuton_input_t INPUT_FEATURES_SCALE_MAX[] =
{ 19612, 19612, 19612, 9743, 8498, 11407 };

/** Defines feature extraction masks used as neuton_nn_features_mask_t,
 *  64 bit for one unique input feature, @ref neuton_nn_features_mask_t to see bitmask
 */
static const neuton_u64_t FEATURES_EXTRACTION_MASK[] =
{ 0x2a8109300000000, 0x288d3dd00000000, 0x838451b00000000, 0xc47d200000000,
 0xfc081ba00000000, 0x32c403200000000 };

/** Defines arguments used while feature extraction
 */
static const neuton_input_t FEATURES_EXTRACTION_ARGUMENTS[] =
{ 4, 2, 4, 2, 2, 2 };

/** Defines extracted features MIN scaling factor
 */
static const neuton_feature_t EXTRACTED_FEATURES_SCALE_MIN[] =
{ -19612, -9561, 3, 5, 10, 120, 100, -489, 894, -19612, 18, -8241, 3, 5373,
 4, 77, 10, 9, 62, 3, 180, -227, 1023, -19612, -9419, -9434, 5, 170, 0,
 168, 190, 0, 100, -9912, -131, 0, 0, 0, 2, 0, 0, 1, 0, 0, -114, -1055, 1,
 -18671, 1, 2, 0, 10, 163, 0, 0, -666, -2539, -521, 1, -13570, 1, 0, 140,
 100, 0, 0 };

/** Defines extracted features MAX scaling factor
 */
static const neuton_feature_t EXTRACTED_FEATURES_SCALE_MAX[] =
{ 9821, 19612, 4773, 5760, 21937, 740, 316, 1002, 25423, 6687, 39224, 9338,
 6161, 88842, 7272, 9382, 686, 30099, 9338, 2405, 840, 999, 18866, 3651,
 19612, 6070, 5412, 9434, 151, 9434, 780, 100, 534, 7205, 9743, 3779,
 409968, 4648, 5072, 222, 222, 3463, 1000, 980, 8498, 1032, 1659, 13994,
 2493, 2677, 600, 15398, 1009, 1000, 13392, 661, 2156, 11407, 1829, 19016,
 2066, 1000, 860, 614, 1046, 28169 };
