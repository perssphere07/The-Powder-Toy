#include "ColourPickerActivity.h"

#include "gui/interface/Textbox.h"
#include "gui/interface/Slider.h"
#include "gui/interface/Button.h"
#include "gui/interface/Label.h"
#include "gui/Style.h"

#include "graphics/Graphics.h"

#include "Misc.h"

#include <SDL.h>

ColourPickerActivity::ColourPickerActivity(ui::Colour initialColour, OnPicked onPicked_) :
	WindowActivity(ui::Point(-1, -1), ui::Point(362, 228)),
	currentHue(0),
	currentSaturation(0),
	currentValue(0),
	mouseDown(false),
	onPicked(onPicked_)
{
	auto colourChange = [this] {
		int r, g, b, alpha;
		r = rValue->GetText().ToNumber<int>(true);
		g = gValue->GetText().ToNumber<int>(true);
		b = bValue->GetText().ToNumber<int>(true);
		alpha = aValue->GetText().ToNumber<int>(true);
		if (r > 255)
			r = 255;
		if (g > 255)
			g = 255;
		if (b > 255)
			b = 255;
		if (alpha > 255)
			alpha = 255;

		RGB_to_HSV(r, g, b, &currentHue, &currentSaturation, &currentValue);
		currentAlpha = alpha;
		UpdateTextboxes(r, g, b, alpha);
		UpdateSliders();
	};

	auto colourChangeSlider = [this] {
		int r, g, b;
		currentValue = vSlider->GetValue();

		HSV_to_RGB(currentHue, currentSaturation, currentValue, &r, &g, &b);
		UpdateTextboxes(r, g, b, currentAlpha);
		UpdateSliders();
	};


	vSlider = new ui::Slider(ui::Point(10, 201), ui::Point(182, 17), 255);
	vSlider->SetActionCallback({ colourChangeSlider });
	AddComponent(vSlider);


	hexValue = new::ui::Label(ui::Point(240, 11), ui::Point(53, 17), "#FFFFFF");
	hexValue->Appearance.HorizontalAlign = ui::Appearance::AlignLeft;
	AddComponent(hexValue);

	rValue = new ui::Textbox(ui::Point(240, 38), ui::Point(50, 17), "255");
	rValue->Appearance.HorizontalAlign = ui::Appearance::AlignLeft;
	rValue->SetActionCallback({ colourChange });
	rValue->SetLimit(3);
	rValue->SetInputType(ui::Textbox::Number);
	AddComponent(rValue);

	gValue = new ui::Textbox(ui::Point(240, 65), ui::Point(50, 17), "255");
	gValue->Appearance.HorizontalAlign = ui::Appearance::AlignLeft;
	gValue->SetActionCallback({ colourChange });
	gValue->SetLimit(3);
	gValue->SetInputType(ui::Textbox::Number);
	AddComponent(gValue);

	bValue = new ui::Textbox(ui::Point(240, 92), ui::Point(50, 17), "255");
	bValue->Appearance.HorizontalAlign = ui::Appearance::AlignLeft;
	bValue->SetActionCallback({ colourChange });
	bValue->SetLimit(3);
	bValue->SetInputType(ui::Textbox::Number);
	AddComponent(bValue);

	aValue = new ui::Textbox(ui::Point(240, 119), ui::Point(50, 17), "255");
	aValue->Appearance.HorizontalAlign = ui::Appearance::AlignLeft;
	aValue->SetActionCallback({ colourChange });
	aValue->SetLimit(3);
	aValue->SetInputType(ui::Textbox::Number);
	AddComponent(aValue);

	redLabel = new::ui::Label(ui::Point(300, 38), ui::Point(15, 17), "Red");
	greenLabel = new::ui::Label(ui::Point(300, 65), ui::Point(24, 17), "Green");
	blueLabel = new::ui::Label(ui::Point(300, 92), ui::Point(19, 17), "Blue");
	alphaLabel = new::ui::Label(ui::Point(300, 119), ui::Point(34, 17), "Opacity");
	AddComponent(redLabel);
	AddComponent(greenLabel);
	AddComponent(blueLabel);
	AddComponent(alphaLabel);

	ui::Button * doneButton = new ui::Button(ui::Point(Size.X-51, Size.Y-28), ui::Point(40, 17), "Done");
	doneButton->SetActionCallback({ [this] {
		int Red, Green, Blue;
		Red = rValue->GetText().ToNumber<int>(true);
		Green = gValue->GetText().ToNumber<int>(true);
		Blue = bValue->GetText().ToNumber<int>(true);
		ui::Colour col(Red, Green, Blue, currentAlpha);
		if (onPicked)
			onPicked(col);
		Exit();
	} });
	AddComponent(doneButton);
	SetOkayButton(doneButton);

	RGB_to_HSV(initialColour.Red, initialColour.Green, initialColour.Blue, &currentHue, &currentSaturation, &currentValue);
	currentAlpha = initialColour.Alpha;
	UpdateTextboxes(initialColour.Red, initialColour.Green, initialColour.Blue, initialColour.Alpha);
	UpdateSliders();
}

void ColourPickerActivity::UpdateTextboxes(int r, int g, int b, int a)
{
	rValue->SetText(String::Build(r));
	gValue->SetText(String::Build(g));
	bValue->SetText(String::Build(b));
	aValue->SetText(String::Build(a));
	hexValue->SetText(String::Build(Format::Hex(), Format::Uppercase(), Format::Width(2), "#", r, g, b));
}

void ColourPickerActivity::UpdateSliders()
{
	vSlider->SetValue(currentValue);

	int r, g, b;

	//Value gradient
	HSV_to_RGB(currentHue, currentSaturation, 255, &r, &g, &b);
	vSlider->SetColour(ui::Colour(0, 0, 0), ui::Colour(r, g, b));
}

void ColourPickerActivity::OnTryExit(ExitMethod method)
{
	Exit();
}

void ColourPickerActivity::OnMouseMove(int x, int y, int dx, int dy)
{
	if(mouseDown)
	{
		x -= Position.X+11;
		y -= Position.Y+11;

		currentHue = (int)(x / 180.0f * 359.0f);
		currentSaturation = 255 - (int)(y / 180.0f * 255.0f);

		if(currentSaturation > 255)
			currentSaturation = 255;
		if(currentSaturation < 0)
			currentSaturation = 0;
		if(currentHue > 359)
			currentHue = 359;
		if(currentHue < 0)
			currentHue = 0;
	}

	if(mouseDown)
	{
		int cr, cg, cb;
		HSV_to_RGB(currentHue, currentSaturation, currentValue, &cr, &cg, &cb);
		UpdateTextboxes(cr, cg, cb, currentAlpha);
		UpdateSliders();
	}
}

void ColourPickerActivity::OnMouseDown(int x, int y, unsigned button)
{
	x -= Position.X+11;
	y -= Position.Y+11;
	if(x >= 0 && x < 180 && y >= 0 && y <= 180)
	{
		mouseDown = true;
		currentHue = (int)(x / 180.0f * 359.0f);
		currentSaturation = 255 - (int)(y / 180.0f * 255.0f);

		if(currentSaturation > 255)
			currentSaturation = 255;
		if(currentSaturation < 0)
			currentSaturation = 0;
		if(currentHue > 359)
			currentHue = 359;
		if(currentHue < 0)
			currentHue = 0;
	}

	if(mouseDown)
	{
		int cr, cg, cb;
		HSV_to_RGB(currentHue, currentSaturation, currentValue, &cr, &cg, &cb);
		UpdateTextboxes(cr, cg, cb, currentAlpha);
		UpdateSliders();
	}
}

void ColourPickerActivity::OnMouseUp(int x, int y, unsigned button)
{
	if(mouseDown)
	{
		int cr, cg, cb;
		HSV_to_RGB(currentHue, currentSaturation, currentValue, &cr, &cg, &cb);
		UpdateTextboxes(cr, cg, cb, currentAlpha);
		UpdateSliders();
	}

	if(mouseDown)
	{
		mouseDown = false;
		x -= Position.X+11;
		y -= Position.Y+11;

		currentHue = (int)(x / 180.0f * 359.0f);
		currentSaturation = 255 - (int)(y / 180.0f * 255.0f);

		if(currentSaturation > 255)
			currentSaturation = 255;
		if(currentSaturation < 0)
			currentSaturation = 0;
		if(currentHue > 359)
			currentHue = 359;
		if(currentHue < 0)
			currentHue = 0;
	}
}

void ColourPickerActivity::OnKeyPress(int key, int scan, bool repeat, bool shift, bool ctrl, bool alt)
{
	if (repeat)
		return;
	if (key == SDLK_TAB)
	{
		if (rValue->IsFocused())
			gValue->TabFocus();
		else if (gValue->IsFocused())
			bValue->TabFocus();
		else if (bValue->IsFocused())
			aValue->TabFocus();
		else if (aValue->IsFocused())
			rValue->TabFocus();
	}
}

void ColourPickerActivity::OnDraw()
{
	Graphics * g = GetGraphics();

	int currentRed = 0;
	int currentGreen = 0;
	int currentBlue = 0;
	HSV_to_RGB(currentHue, currentSaturation, currentValue, &currentRed, &currentGreen, &currentBlue);

	g->BlendFilledRect(RectSized(Position - Vec2{ 2, 2 }, Size + Vec2{ 3, 3 }), 0x000000_rgb .WithAlpha(currentAlpha));
	g->DrawRect(RectSized(Position, Size), 0xFFFFFF_rgb);

	auto offset = Position + Vec2{ 11, 11 };

	//draw color square
	int lastx = -1, currx = 0;
	for (int saturation = 0; saturation <= 255; saturation++)
	{
		for (int hue = 0; hue <= 359; hue += 2)
		{
			currx = offset.X + (int)(hue / 359.0f * 180.0f);
			if (currx == lastx)
				continue;
			lastx = currx;
			int cr = 0;
			int cg = 0;
			int cb = 0;
			HSV_to_RGB(hue, 255 - saturation, currentValue, &cr, &cg, &cb);
			g->BlendPixel({ currx, offset.Y + (int)(saturation / 255.0f * 180.0f) }, RGBA<uint8_t>(cr, cg, cb, currentAlpha));
		}
	}
	g->BlendRect(RectSized(offset, Vec2{ 180, 180 }), 0xFFFFFF_rgb .WithAlpha(31));

	g->BlendFilledRect(RectSized(offset + Vec2{ 190, 0 }, Vec2{ 30, 206 }), RGBA<uint8_t>(currentRed, currentGreen, currentBlue, currentAlpha));
	g->BlendRect(RectSized(offset + Vec2{ 190, 0 }, Vec2{ 30, 206 }), 0xFFFFFF_rgb .WithAlpha(31));

	//draw color square pointer
	int currentHueX = (int)(currentHue / 359.0f * 180.f);
	int currentSaturationY = (int)((255 - currentSaturation) / 255.0f * 180.f);
	g->XorLine(offset + Vec2{ currentHueX, currentSaturationY-5 }, offset + Vec2{ currentHueX, currentSaturationY-1 });
	g->XorLine(offset + Vec2{ currentHueX, currentSaturationY+1 }, offset + Vec2{ currentHueX, currentSaturationY+5 });
	g->XorLine(offset + Vec2{ currentHueX-5, currentSaturationY }, offset + Vec2{ currentHueX-1, currentSaturationY });
	g->XorLine(offset + Vec2{ currentHueX+1, currentSaturationY }, offset + Vec2{ currentHueX+5, currentSaturationY });

}
