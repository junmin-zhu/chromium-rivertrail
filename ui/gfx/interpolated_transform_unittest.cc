// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/gfx/interpolated_transform.h"

#include "base/basictypes.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "ui/gfx/rect.h"

namespace {

void CheckApproximatelyEqual(const gfx::Transform& lhs,
                             const gfx::Transform& rhs) {
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      EXPECT_FLOAT_EQ(lhs.matrix().get(i, j), rhs.matrix().get(i, j));
    }
  }
}

float NormalizeAngle(float angle) {
  while (angle < 0.0f) {
    angle += 360.0f;
  }
  while (angle > 360.0f) {
    angle -= 360.0f;
  }
  return angle;
}

} // namespace

TEST(InterpolatedTransformTest, InterpolatedRotation) {
  ui::InterpolatedRotation interpolated_rotation(0, 100);
  ui::InterpolatedRotation interpolated_rotation_diff_start_end(
      0, 100, 100, 200);

  for (int i = 0; i <= 100; ++i) {
    gfx::Transform rotation;
    rotation.SetRotate(i);
    gfx::Transform interpolated = interpolated_rotation.Interpolate(i / 100.0f);
    CheckApproximatelyEqual(rotation, interpolated);
    interpolated = interpolated_rotation_diff_start_end.Interpolate(i + 100);
    CheckApproximatelyEqual(rotation, interpolated);
  }
}

TEST(InterpolatedTransformTest, InterpolatedScale) {
  ui::InterpolatedScale interpolated_scale(gfx::Point3f(0, 0, 0),
                                           gfx::Point3f(100, 100, 100));
  ui::InterpolatedScale interpolated_scale_diff_start_end(
      gfx::Point3f(0, 0, 0), gfx::Point3f(100, 100, 100), 100, 200);

  for (int i = 0; i <= 100; ++i) {
    gfx::Transform scale;
    scale.SetScale(i, i);
    gfx::Transform interpolated = interpolated_scale.Interpolate(i / 100.0f);
    CheckApproximatelyEqual(scale, interpolated);
    interpolated = interpolated_scale_diff_start_end.Interpolate(i + 100);
    CheckApproximatelyEqual(scale, interpolated);
  }
}

TEST(InterpolatedTransformTest, InterpolatedTranslate) {
  ui::InterpolatedTranslation interpolated_xform(gfx::Point(0, 0),
                                                 gfx::Point(100, 100));

  ui::InterpolatedTranslation interpolated_xform_diff_start_end(
      gfx::Point(0, 0), gfx::Point(100, 100), 100, 200);

  for (int i = 0; i <= 100; ++i) {
    gfx::Transform xform;
    xform.SetTranslate(i, i);
    gfx::Transform interpolated = interpolated_xform.Interpolate(i / 100.0f);
    CheckApproximatelyEqual(xform, interpolated);
    interpolated = interpolated_xform_diff_start_end.Interpolate(i + 100);
    CheckApproximatelyEqual(xform, interpolated);
  }
}

TEST(InterpolatedTransformTest, InterpolatedRotationAboutPivot) {
  gfx::Point pivot(100, 100);
  gfx::Point above_pivot(100, 200);
  ui::InterpolatedRotation rot(0, 90);
  ui::InterpolatedTransformAboutPivot interpolated_xform(
      pivot,
      new ui::InterpolatedRotation(0, 90));
  gfx::Transform result = interpolated_xform.Interpolate(0.0f);
  CheckApproximatelyEqual(gfx::Transform(), result);
  result = interpolated_xform.Interpolate(1.0f);
  gfx::Point expected_result = pivot;
  result.TransformPoint(pivot);
  EXPECT_EQ(expected_result, pivot);
  expected_result = gfx::Point(0, 100);
  result.TransformPoint(above_pivot);
  EXPECT_EQ(expected_result, above_pivot);
}

TEST(InterpolatedTransformTest, InterpolatedScaleAboutPivot) {
  gfx::Point pivot(100, 100);
  gfx::Point above_pivot(100, 200);
  ui::InterpolatedTransformAboutPivot interpolated_xform(
      pivot,
      new ui::InterpolatedScale(gfx::Point3f(1, 1, 1), gfx::Point3f(2, 2, 2)));
  gfx::Transform result = interpolated_xform.Interpolate(0.0f);
  CheckApproximatelyEqual(gfx::Transform(), result);
  result = interpolated_xform.Interpolate(1.0f);
  gfx::Point expected_result = pivot;
  result.TransformPoint(pivot);
  EXPECT_EQ(expected_result, pivot);
  expected_result = gfx::Point(100, 300);
  result.TransformPoint(above_pivot);
  EXPECT_EQ(expected_result, above_pivot);
}

TEST(InterpolatedTransformTest, FactorTRS) {
  for (int degrees = 0; degrees < 360; ++degrees) {
    // build a transformation matrix.
    gfx::Transform transform;
    transform.SetScale(degrees + 1, 2 * degrees + 1);
    transform.ConcatRotate(degrees);
    transform.ConcatTranslate(degrees * 2, -degrees * 3);

    // factor the matrix
    gfx::Point translation;
    float rotation;
    gfx::Point3f scale;
    bool success = ui::InterpolatedTransform::FactorTRS(transform,
                                                        &translation,
                                                        &rotation,
                                                        &scale);
    EXPECT_TRUE(success);
    EXPECT_FLOAT_EQ(translation.x(), degrees * 2);
    EXPECT_FLOAT_EQ(translation.y(), -degrees * 3);
    EXPECT_FLOAT_EQ(NormalizeAngle(rotation), degrees);
    EXPECT_FLOAT_EQ(scale.x(), degrees + 1);
    EXPECT_FLOAT_EQ(scale.y(), 2 * degrees + 1);
  }
}

ui::InterpolatedTransform* GetScreenRotation(int degrees, bool reversed) {
  gfx::Point old_pivot;
  gfx::Point new_pivot;

  int width = 1920;
  int height = 180;

  switch (degrees) {
    case 90:
      new_pivot = gfx::Point(width, 0);
      break;
    case -90:
      new_pivot = gfx::Point(0, height);
      break;
    case 180:
    case 360:
      new_pivot = old_pivot = gfx::Point(width / 2, height / 2);
      break;
  }

  scoped_ptr<ui::InterpolatedTransform> rotation(
      new ui::InterpolatedTransformAboutPivot(
          old_pivot,
          new ui::InterpolatedRotation(reversed ? degrees : 0,
                                       reversed ? 0 : degrees)));

  scoped_ptr<ui::InterpolatedTransform> translation(
      new ui::InterpolatedTranslation(
          gfx::Point(0, 0),
          gfx::Point(new_pivot.x() - old_pivot.x(),
                     new_pivot.y() - old_pivot.y())));

  float scale_factor = 0.9f;
  scoped_ptr<ui::InterpolatedTransform> scale_down(
      new ui::InterpolatedScale(1.0f, scale_factor, 0.0f, 0.5f));

  scoped_ptr<ui::InterpolatedTransform> scale_up(
      new ui::InterpolatedScale(1.0f, 1.0f / scale_factor, 0.5f, 1.0f));

  scoped_ptr<ui::InterpolatedTransform> to_return(
      new ui::InterpolatedConstantTransform(gfx::Transform()));

  scale_up->SetChild(scale_down.release());
  translation->SetChild(scale_up.release());
  rotation->SetChild(translation.release());
  to_return->SetChild(rotation.release());
  to_return->SetReversed(reversed);

  return to_return.release();
}

TEST(InterpolatedTransformTest, ScreenRotationEndsCleanly) {
  for (int i = 0; i < 2; ++i) {
    for (int degrees = -360; degrees <= 360; degrees += 90) {
      const bool reversed = i == 1;
      scoped_ptr<ui::InterpolatedTransform> screen_rotation(
          GetScreenRotation(degrees, reversed));
      gfx::Transform interpolated = screen_rotation->Interpolate(1.0f);
      SkMatrix44& m = interpolated.matrix();
      // Upper-left 3x3 matrix should all be 0, 1 or -1.
      for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
          float entry = m.get(row, col);
          EXPECT_TRUE(entry == 0 || entry == 1 || entry == -1);
        }
      }
    }
  }
}

ui::InterpolatedTransform* GetMaximize() {
  gfx::Rect target_bounds(0, 0, 1920, 1080);
  gfx::Rect initial_bounds(30, 1000, 192, 108);

  float scale_x = static_cast<float>(
      target_bounds.height()) / initial_bounds.width();
  float scale_y = static_cast<float>(
      target_bounds.width()) / initial_bounds.height();

  scoped_ptr<ui::InterpolatedTransform> scale(
      new ui::InterpolatedScale(gfx::Point3f(1, 1, 1),
                                gfx::Point3f(scale_x, scale_y, 1)));

  scoped_ptr<ui::InterpolatedTransform> translation(
      new ui::InterpolatedTranslation(
          gfx::Point(),
          gfx::Point(target_bounds.x() - initial_bounds.x(),
                     target_bounds.y() - initial_bounds.y())));

  scoped_ptr<ui::InterpolatedTransform> rotation(
      new ui::InterpolatedRotation(0, 4.0f));

  scoped_ptr<ui::InterpolatedTransform> rotation_about_pivot(
      new ui::InterpolatedTransformAboutPivot(
          gfx::Point(initial_bounds.width() * 0.5,
                     initial_bounds.height() * 0.5),
          rotation.release()));

  scale->SetChild(translation.release());
  rotation_about_pivot->SetChild(scale.release());

  rotation_about_pivot->SetReversed(true);

  return rotation_about_pivot.release();
}

TEST(InterpolatedTransformTest, MaximizeEndsCleanly) {
  scoped_ptr<ui::InterpolatedTransform> maximize(GetMaximize());
  gfx::Transform interpolated = maximize->Interpolate(1.0f);
  SkMatrix44& m = interpolated.matrix();
  // Upper-left 3x3 matrix should all be 0, 1 or -1.
  for (int row = 0; row < 3; ++row) {
    for (int col = 0; col < 3; ++col) {
      float entry = m.get(row, col);
      EXPECT_TRUE(entry == 0 || entry == 1 || entry == -1);
    }
  }
}

