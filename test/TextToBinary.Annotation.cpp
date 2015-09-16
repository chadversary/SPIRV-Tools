// Copyright (c) 2015 The Khronos Group Inc.
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and/or associated documentation files (the
// "Materials"), to deal in the Materials without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Materials, and to
// permit persons to whom the Materials are furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Materials.
//
// MODIFICATIONS TO THIS FILE MAY MEAN IT NO LONGER ACCURATELY REFLECTS
// KHRONOS STANDARDS. THE UNMODIFIED, NORMATIVE VERSIONS OF KHRONOS
// SPECIFICATIONS AND HEADER INFORMATION ARE LOCATED AT
//    https://www.khronos.org/registry/
//
// THE MATERIALS ARE PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// MATERIALS OR THE USE OR OTHER DEALINGS IN THE MATERIALS.

// Assembler tests for instructions in the "Annotation" section of the
// SPIR-V spec.

#include "UnitSPIRV.h"

#include <sstream>

#include "gmock/gmock.h"
#include "TestFixture.h"

namespace {

using spvtest::MakeInstruction;
using spvtest::MakeVector;
using ::testing::Eq;
using test_fixture::TextToBinaryTest;

// Test OpDecorate

// A single test case for a simple OpDecorate.
// Simple decorations have no operands, or just literal numbers
// as operands.
struct DecorateSimpleCase {
  // Place the enum value first, so it's easier to read the binary dumps when
  // the test fails.
  const spv::Decoration decoration;
  const std::string name;
  const std::vector<uint32_t> operands;
};

using OpDecorateSimpleTest =
    test_fixture::TextToBinaryTestBase<::testing::TestWithParam<DecorateSimpleCase>>;

TEST_P(OpDecorateSimpleTest, AnySimpleDecoration) {
  // This string should assemble, but should not validate.
  std::stringstream input;
  input << "OpDecorate %1 " << GetParam().name;
  for (auto operand : GetParam().operands) input << " " << operand;
  std::vector<uint32_t> expected_operands{1, uint32_t(GetParam().decoration)};
  expected_operands.insert(expected_operands.end(), GetParam().operands.begin(),
                           GetParam().operands.end());
  EXPECT_THAT(CompiledInstructions(input.str()),
              Eq(MakeInstruction(spv::OpDecorate, expected_operands)));
}

#define CASE(NAME) spv::Decoration##NAME, #NAME
INSTANTIATE_TEST_CASE_P(TextToBinaryDecorateSimple, OpDecorateSimpleTest,
                        ::testing::ValuesIn(std::vector<DecorateSimpleCase>{
                          // The operand literal values are arbitrarily chosen,
                          // but there are the right number of them.
                            {CASE(RelaxedPrecision), {}},
                            {CASE(SpecId), {100}},
                            {CASE(Block), {}},
                            {CASE(BufferBlock), {}},
                            {CASE(RowMajor), {}},
                            {CASE(ColMajor), {}},
                            {CASE(ArrayStride), {4}},
                            {CASE(MatrixStride), {16}},
                            {CASE(GLSLShared), {}},
                            {CASE(GLSLPacked), {}},
                            {CASE(CPacked), {}},
                            {CASE(Smooth), {}},
                            {CASE(Noperspective), {}},
                            {CASE(Flat), {}},
                            {CASE(Patch), {}},
                            {CASE(Centroid), {}},
                            {CASE(Sample), {}},
                            {CASE(Invariant), {}},
                            {CASE(Restrict), {}},
                            {CASE(Aliased), {}},
                            {CASE(Volatile), {}},
                            {CASE(Constant), {}},
                            {CASE(Coherent), {}},
                            {CASE(Nonwritable), {}},
                            {CASE(Nonreadable), {}},
                            {CASE(Uniform), {}},
                            {CASE(NoStaticUse), {}},
                            {CASE(SaturatedConversion), {}},
                            {CASE(Stream), {2}},
                            {CASE(Location), {6}},
                            {CASE(Component), {3}},
                            {CASE(Index), {14}},
                            {CASE(Binding), {19}},
                            {CASE(DescriptorSet), {7}},
                            {CASE(Offset), {12}},
                            {CASE(XfbBuffer), {1}},
                            {CASE(XfbStride), {8}},
                        }));
#undef CASE

// A single test case for an enum decoration.
struct DecorateEnumCase {
  // Place the enum value first, so it's easier to read the binary dumps when
  // the test fails.
  const uint32_t value;  // The value within the enum, e.g. Position
  const std::string name;
  const uint32_t enum_value;  // Which enum, e.g. BuiltIn
  const std::string enum_name;
};

using OpDecorateEnumTest = test_fixture::TextToBinaryTestBase<
    ::testing::TestWithParam<DecorateEnumCase>>;

TEST_P(OpDecorateEnumTest, AnyEnumDecoration) {
  // This string should assemble, but should not validate.
  std::string input = "OpDecorate %1 " + GetParam().enum_name + " " + GetParam().name;
  EXPECT_THAT(CompiledInstructions(input),
              Eq(MakeInstruction(spv::OpDecorate, {1, GetParam().enum_value, GetParam().value})));
}

// Test OpDecorate BuiltIn.
// clang-format off
#define CASE(NAME) \
  { spv::BuiltIn##NAME, #NAME, spv::DecorationBuiltIn, "BuiltIn" }
INSTANTIATE_TEST_CASE_P(TextToBinaryDecorateBuiltIn, OpDecorateEnumTest,
                        ::testing::ValuesIn(std::vector<DecorateEnumCase>{
                            CASE(Position),
                            CASE(PointSize),
                            CASE(ClipDistance),
                            CASE(CullDistance),
                            CASE(VertexId),
                            CASE(InstanceId),
                            CASE(PrimitiveId),
                            CASE(InvocationId),
                            CASE(Layer),
                            CASE(ViewportIndex),
                            CASE(TessLevelOuter),
                            CASE(TessLevelInner),
                            CASE(TessCoord),
                            CASE(PatchVertices),
                            CASE(FragCoord),
                            CASE(PointCoord),
                            CASE(FrontFacing),
                            CASE(SampleId),
                            CASE(SamplePosition),
                            CASE(SampleMask),
                            CASE(FragColor),
                            CASE(FragDepth),
                            CASE(HelperInvocation),
                            CASE(NumWorkgroups),
                            CASE(WorkgroupSize),
                            CASE(WorkgroupId),
                            CASE(LocalInvocationId),
                            CASE(GlobalInvocationId),
                            CASE(LocalInvocationIndex),
                            CASE(WorkDim),
                            CASE(GlobalSize),
                            CASE(EnqueuedWorkgroupSize),
                            CASE(GlobalOffset),
                            CASE(GlobalLinearId),
                            CASE(WorkgroupLinearId),
                            CASE(SubgroupSize),
                            CASE(SubgroupMaxSize),
                            CASE(NumSubgroups),
                            CASE(NumEnqueuedSubgroups),
                            CASE(SubgroupId),
                            CASE(SubgroupLocalInvocationId),
                        }));
#undef CASE
// clang-format on

// Test OpDecorate FuncParamAttr
// clang-format off
#define CASE(NAME) \
  { spv::FunctionParameterAttribute##NAME, #NAME, spv::DecorationFuncParamAttr, "FuncParamAttr" }
INSTANTIATE_TEST_CASE_P(TextToBinaryDecorateFuncParamAttr, OpDecorateEnumTest,
                        ::testing::ValuesIn(std::vector<DecorateEnumCase>{
                            CASE(Zext),
                            CASE(Sext),
                            CASE(ByVal),
                            CASE(Sret),
                            CASE(NoAlias),
                            CASE(NoCapture),
                            CASE(NoWrite),
                            CASE(NoReadWrite),
                      }));
#undef CASE
// clang-format on

// Test OpDecorate FPRoundingMode
// clang-format off
#define CASE(NAME) \
  { spv::FPRoundingMode##NAME, #NAME, spv::DecorationFPRoundingMode, "FPRoundingMode" }
INSTANTIATE_TEST_CASE_P(TextToBinaryDecorateFPRoundingMode, OpDecorateEnumTest,
                        ::testing::ValuesIn(std::vector<DecorateEnumCase>{
                            CASE(RTE),
                            CASE(RTZ),
                            CASE(RTP),
                            CASE(RTN),
                      }));
#undef CASE
// clang-format on

// Test OpDecorate FPFastMathMode.
// These can by named enums for the single-bit masks.  However, we don't support
// symbolic combinations of the masks.  Rather, they can use !<immediate>
// syntax, e.g. !0x3

// clang-format off
#define CASE(ENUM,NAME) \
  { spv::FPFastMathMode##ENUM, #NAME, spv::DecorationFPFastMathMode, "FPFastMathMode" }
INSTANTIATE_TEST_CASE_P(TextToBinaryDecorateFPFastMathMode, OpDecorateEnumTest,
                        ::testing::ValuesIn(std::vector<DecorateEnumCase>{
                            CASE(MaskNone, None),
                            CASE(NotNaNMask, NotNaN),
                            CASE(NotInfMask, NotInf),
                            CASE(NSZMask, NSZ),
                            CASE(AllowRecipMask, AllowRecip),
                            CASE(FastMask, Fast),
                      }));
#undef CASE
// clang-format on

TEST_F(TextToBinaryTest, CombinedFPFastMathMask) {
  // Sample a single combination.  This ensures we've integrated
  // the instruction parsing logic with spvTextParseMask.
  const std::string input = "OpDecorate %1 FPFastMathMode NotNaN|NotInf|NSZ";
  const uint32_t expected_enum = spv::DecorationFPFastMathMode;
  const uint32_t expected_mask = spv::FPFastMathModeNotNaNMask |
                                 spv::FPFastMathModeNotInfMask |
                                 spv::FPFastMathModeNSZMask;
  EXPECT_THAT(
      CompiledInstructions(input),
      Eq(MakeInstruction(spv::OpDecorate, {1, expected_enum, expected_mask})));
}

// Test OpDecorate Linkage

// A single test case for a linkage
struct DecorateLinkageCase {
  const uint32_t linkage_type_value;
  const std::string linkage_type_name;
  const std::string external_name;
};

using OpDecorateLinkageTest = test_fixture::TextToBinaryTestBase<
    ::testing::TestWithParam<DecorateLinkageCase>>;

TEST_P(OpDecorateLinkageTest, AnyLinkageDecoration) {
  // This string should assemble, but should not validate.
  std::string input = "OpDecorate %1 LinkageAttributes \"" + GetParam().external_name +
                      "\" " + GetParam().linkage_type_name;
  std::vector<uint32_t> expected_operands{1, spv::DecorationLinkageAttributes};
  std::vector<uint32_t> encoded_external_name =
      MakeVector(GetParam().external_name);
  expected_operands.insert(expected_operands.end(),
                           encoded_external_name.begin(),
                           encoded_external_name.end());
  expected_operands.push_back(GetParam().linkage_type_value);
  EXPECT_THAT(CompiledInstructions(input),
              Eq(MakeInstruction(spv::OpDecorate, expected_operands)));
}

// clang-format off
#define CASE(ENUM) spv::LinkageType##ENUM, #ENUM
INSTANTIATE_TEST_CASE_P(TextToBinaryDecorateLinkage, OpDecorateLinkageTest,
                        ::testing::ValuesIn(std::vector<DecorateLinkageCase>{
                            { CASE(Import), "a" },
                            { CASE(Export), "foo" },
                            { CASE(Import), "some kind of long name with spaces etc." },
                            // TODO(dneto): utf-8, escaping, quoting cases.
                      }));
#undef CASE
// clang-format on

// TODO(dneto): OpMemberDecorate
// TODO(dneto): OpDecorationGroup
// TODO(dneto): OpGroupDecorate
// TODO(dneto): OpGroupMemberDecorate

}  // anonymous namespace
