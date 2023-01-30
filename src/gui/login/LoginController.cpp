#include "LoginController.h"

#include "client/Client.h"
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

User LoginController::GetUser()
{
	return loginModel->GetUser();
}

void LoginController::Exit()
{
	loginView->CloseActiveWindow();
	Client::Ref().SetAuthUser(loginModel->GetUser());
	if (onDone)
		onDone();
	HasExited = true;
}

void LoginController::CreateAccount()
{
	ByteString uri = ByteString::Build(SCHEME, SERVER, "/Register.html");
	Platform::OpenURI(uri);
}

LoginController::~LoginController()
{
	loginView->CloseActiveWindow();
	delete loginModel;
	delete loginView;
}

