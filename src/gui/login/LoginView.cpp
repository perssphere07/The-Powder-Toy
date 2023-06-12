#include "LoginView.h"
#include "LoginModel.h"
#include "LoginController.h"
#include "graphics/Graphics.h"
#include "gui/interface/Button.h"
#include "gui/interface/Label.h"
#include "gui/interface/Textbox.h"
#include "gui/Style.h"
#include "client/Client.h"
#include "Misc.h"
#include <SDL.h>

LoginView::LoginView():
	ui::Window(ui::Point(-1, -1), ui::Point(256, 128)),
	loginButton(new ui::Button(ui::Point(256-129, 87-17), ui::Point(129, 17), "Sign in")),
	cancelButton(new ui::Button(ui::Point(0, 87-17), ui::Point(128, 17), "Sign Out")),
	createAccountButton(new ui::Button(ui::Point(91, 73), ui::Point(61, 17), "Create One!")),
	titleLabel(new ui::Label(ui::Point(8, 6), ui::Point(256-16, 16), "Sign in")),
	createAccountLabel(new ui::Label(ui::Point(29, 74), ui::Point(128, 16), "No account?")),
	infoLabel(new ui::Label(ui::Point(8, 67), ui::Point(256-16, 16), "")),
	usernameField(new ui::Textbox(ui::Point(32, 30), ui::Point(192, 17), Client::Ref().GetAuthUser().Username.FromUtf8(), "Username")),
	passwordField(new ui::Textbox(ui::Point(32, 56), ui::Point(192, 17), "", "Password")),
	targetSize(0, 0)
{
	targetSize = Size;
	FocusComponent(usernameField);

	infoLabel->Appearance.HorizontalAlign = ui::Appearance::AlignCentre;
	infoLabel->Appearance.VerticalAlign = ui::Appearance::AlignTop;
	infoLabel->SetMultiline(true);
	infoLabel->Visible = false;
	AddComponent(infoLabel);

	AddComponent(loginButton);
	SetOkayButton(loginButton);
	loginButton->Appearance.HorizontalAlign = ui::Appearance::AlignRight;
	loginButton->Appearance.VerticalAlign = ui::Appearance::AlignMiddle;
	loginButton->Appearance.TextInactive = style::Colour::ConfirmButton;
	loginButton->SetActionCallback({ [this] {
		c->Login(usernameField->GetText().ToUtf8(), passwordField->GetText().ToUtf8());
	} });
	AddComponent(cancelButton);
	cancelButton->Appearance.HorizontalAlign = ui::Appearance::AlignLeft;
	cancelButton->Appearance.VerticalAlign = ui::Appearance::AlignMiddle;
	cancelButton->SetActionCallback({ [this] {
		c->Logout();
	} });
	AddComponent(titleLabel);
	createAccountLabel->Appearance.HorizontalAlign = ui::Appearance::AlignLeft;
	createAccountLabel->Appearance.VerticalAlign = ui::Appearance::AlignMiddle;
	AddComponent(createAccountLabel);
	createAccountButton->Appearance.HorizontalAlign = ui::Appearance::AlignLeft;
	createAccountButton->Appearance.VerticalAlign = ui::Appearance::AlignMiddle;
	createAccountButton->Appearance.TextInactive = style::Colour::Hyperlink;
	createAccountButton->Appearance.Border = 0;
	createAccountButton->SetActionCallback({ [this] { c->CreateAccount(); }});
	AddComponent(createAccountButton);
	titleLabel->Appearance.HorizontalAlign = ui::Appearance::AlignLeft;
	titleLabel->Appearance.VerticalAlign = ui::Appearance::AlignMiddle;

	AddComponent(usernameField);
	usernameField->Appearance.icon = IconContact;
	usernameField->Appearance.HorizontalAlign = ui::Appearance::AlignLeft;
	usernameField->Appearance.VerticalAlign = ui::Appearance::AlignMiddle;
	usernameField->Appearance.Margin.Left -= 1;
	AddComponent(passwordField);
	passwordField->Appearance.icon = IconDialpad;
	passwordField->Appearance.HorizontalAlign = ui::Appearance::AlignLeft;
	passwordField->Appearance.VerticalAlign = ui::Appearance::AlignMiddle;
	passwordField->Appearance.Margin.Left -= 1;
	passwordField->SetHidden(true);
}

void LoginView::OnKeyPress(int key, int scan, bool repeat, bool shift, bool ctrl, bool alt)
{
	if (repeat)
		return;
	switch(key)
	{
	case SDLK_TAB:
		if(IsFocused(usernameField))
			FocusComponent(passwordField);
		else
			FocusComponent(usernameField);
		break;
	}
}

void LoginView::OnTryExit(ExitMethod method)
{
	CloseActiveWindow();
}

void LoginView::NotifyStatusChanged(LoginModel * sender)
{
	if (infoLabel->Visible)
		targetSize.Y = 87;
	infoLabel->SetText(sender->GetStatusText());
	infoLabel->AutoHeight();
	auto notWorking = sender->GetStatus() != loginWorking;
	loginButton->Enabled = notWorking;
	cancelButton->Enabled = notWorking && Client::Ref().GetAuthUser().UserID;
	usernameField->Enabled = notWorking;
	passwordField->Enabled = notWorking;
	if (sender->GetStatusText().length())
	{
		targetSize.Y += infoLabel->Size.Y+2;
		infoLabel->Visible = true;
	}
	if (sender->GetStatus() == loginSucceeded)
	{
		c->Exit();
	}
}

void LoginView::OnTick(float dt)
{
	c->Tick();
	//if(targetSize != Size)
	{
		ui::Point difference = targetSize-Size;
		if(difference.X!=0)
		{
			int xdiff = difference.X/5;
			if(xdiff == 0)
				xdiff = 1*isign(difference.X);
			Size.X += xdiff;
		}
		if(difference.Y!=0)
		{
			int ydiff = difference.Y/5;
			if(ydiff == 0)
				ydiff = 1*isign(difference.Y);
			Size.Y += ydiff;
		}

		loginButton->Position.Y = Size.Y-17;
		cancelButton->Position.Y = Size.Y-17;
	}
}

void LoginView::OnDraw()
{
	Graphics * g = GetGraphics();
	g->DrawFilledRect(RectSized(Position - Vec2{ 1, 1 }, Size + Vec2{ 2, 2 }), 0x000000_rgb);
	g->DrawRect(RectSized(Position, Size), 0xFFFFFF_rgb);
}

LoginView::~LoginView() {
	RemoveComponent(titleLabel);
	RemoveComponent(loginButton);
	RemoveComponent(cancelButton);
	RemoveComponent(createAccountButton);
	RemoveComponent(usernameField);
	RemoveComponent(passwordField);
	RemoveComponent(infoLabel);
	RemoveComponent(createAccountLabel);
	delete cancelButton;
	delete loginButton;
	delete titleLabel;
	delete usernameField;
	delete passwordField;
	delete infoLabel;
}

