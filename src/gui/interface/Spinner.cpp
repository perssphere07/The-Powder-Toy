#include "Spinner.h"
#include "graphics/Graphics.h"
#include <cmath>

using namespace ui;

const double PI = std::acos(-1);

Spinner::Spinner(Point position, Point size):
	Component(position, size), cValue(0),
	tickInternal(0)
{
}
void Spinner::Tick(float dt)
{
	tickInternal++;
	if(tickInternal == 4)
	{
		cValue += 0.25f;//0.05f;
		tickInternal = 0;
	}
}
void Spinner::Draw(const Point& screenPos)
{
	Graphics * g = GetGraphics();
	
	auto base = screenPos + Vec2{ Size.X / 2, Size.Y / 2 - 16 };

	int endAngle = (int)(cValue * 30 + sin(cValue / 3) * 45) % 360;
	int startAngle = (int)(endAngle - (sin(cValue / 3) * 120) - 150) % 360;
	int ringSize = 16;
	int ringWidth = 4;

	if (startAngle > endAngle) endAngle += 360;

	g->DrawFilledRect(RectSized(base - Vec2{ 18, 18 }, Vec2{ 36, 36 }), 0x000000_rgb); // Background of the ProgressRing

	for (double i = 0; i < 360; i += 0.5f)
		for (double j = ringSize; j >= ringSize - ringWidth + 1; j -= 0.5f)
			g->DrawPixel(
				base + Vec2{ (int)(cos((i - 90) * PI / 180) * j),
				(int)(sin((i - 90) * PI / 180) * j) }, 0x3F3F3F_rgb);

	for (double i = startAngle; i <= endAngle; i += 0.5f)
		for (double j = ringSize; j >= ringSize - ringWidth + 1; j -= 0.5f)
			g->DrawPixel(
				base + Vec2{ (int)(cos((i - 90) * PI / 180) * j),
				(int)(sin((i - 90) * PI / 180) * j) }, 0x00BFFF_rgb);

	g->BlendFilledEllipse(
		base + Vec2{ (int)(cos((startAngle - 90) * PI / 180) * (ringSize - ringWidth / 2)),
		(int)(sin((startAngle - 90) * PI / 180) * (ringSize - ringWidth / 2)) },
		Vec2{ ringWidth / 2, ringWidth / 2 }, 0x00BFFF_rgb .WithAlpha(255));
	
	g->BlendFilledEllipse(
		base + Vec2{ (int)(cos((endAngle - 90) * PI / 180) * (ringSize - ringWidth / 2)),
		(int)(sin((endAngle - 90) * PI / 180) * (ringSize - ringWidth / 2)) },
		Vec2{ ringWidth / 2, ringWidth / 2 }, 0x00BFFF_rgb .WithAlpha(255));

	for (double i = 0; i < 360; i += 0.5f)
		if ((base.X + cos((i - 90) * PI / 180) * 12) < base.X + 12 && base.Y + sin((i - 90) * PI / 180) * 12 < base.Y + 12)
			g->DrawPixel(
				base + Vec2{ (int)(cos((i - 90) * PI / 180) * 12),
				(int)(sin((i - 90) * PI / 180) * 12) }, 0x000000_rgb);

	g->DrawRect(RectSized(base - Vec2{ 17, 17 }, Vec2{ 34, 34 }), 0x000000_rgb);
}
Spinner::~Spinner()
{

}
