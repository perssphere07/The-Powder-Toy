#include "LoginController.h"
#include "client/Client.h"
#include "client/http/LoginRequest.h"
#include "client/http/LogoutRequest.h"
#include "common/platform/Platform.h"
#include "LoginView.h"
#include "LoginModel.h"
#include "Config.h"
#include "Controller.h"

LoginController::LoginController(std::function<void ()> onDone_):
	HasExited(false)
{
	loginView = new LoginView();
	loginModel = new LoginModel();

	loginView->AttachController(this);
	loginModel->AddObserver(loginView);

	onDone = onDone_;
}

void LoginController::Login(ByteString username, ByteString password)
{
	loginModel->Login(username, password);
}

void LoginController::Logout()
{
	loginModel->Logout();
}

void LoginController::Tick()
{
	loginModel->Tick();
}

void LoginController::CreateAccount()
{
	ByteString uri = ByteString::Build(SCHEME, SERVER, "/Register.html");
	Platform::OpenURI(uri);
}

void LoginController::Exit()
{
	loginView->CloseActiveWindow();
	if (onDone)
		onDone();
	HasExited = true;
}

LoginController::~LoginController()
{
	delete loginModel;
	if (loginView->CloseActiveWindow())
	{
		delete loginView;
	}
}

