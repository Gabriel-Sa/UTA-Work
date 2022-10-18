/*
  Gabriel de Sa
  UTA ID: 1001676832
  Coding Assignment 5
*/
#include <iostream>
#include </usr/local/include/gtkmm-3.0/gtkmm.h>
#include <string>

using namespace std;

class mainWindow : public Gtk::Window
{
  public:
    mainWindow();
    void cancel_clicked();
    void makeMenuBar(Gtk::Box *vBox);
    void ok_clicked(Gtk::Entry *input);
};

int main(int argc, char *argv[])
{
  auto app = Gtk::Application::create(argc, argv, "");
  mainWindow window;
  return app->run(window);
}

mainWindow::mainWindow()
{
  set_title("Guess The Character!");
  set_default_size(800,400);
  realize();
  Gtk::Box *vBox = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL, 0));
  add(*vBox);
  makeMenuBar(vBox);


  Gtk::Label *Question = Gtk::manage(new Gtk::Label("\n\n Who is this One Piece character? \n"));
  vBox->pack_start(*Question);
  Question->show();

  Gtk::Image *luffyImage = Gtk::manage(new Gtk::Image{"luffy.png"});
  vBox->pack_start(*luffyImage);
  luffyImage->show();

  Gtk::Box *hBox = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 0));
  vBox->add(*hBox);

  Gtk::Box *buttonVBox = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL, 0));
  hBox->add(*buttonVBox);

  Gtk::Button *ok = Gtk::manage(new Gtk::Button("Ok"));
  buttonVBox->pack_start(*ok);
  ok->set_can_default();
  ok->set_size_request(get_width()/2);
  ok->grab_default();
  ok->set_border_width(3);

  Gtk::Button *cancel = Gtk::manage(new Gtk::Button("Cancel"));
  buttonVBox->pack_start(*cancel);
  cancel->set_border_width(3);
  cancel->signal_clicked().connect(sigc::mem_fun(*this, &mainWindow::cancel_clicked));

  Gtk::Box *inputVBox = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL, 0));
  hBox->add(*inputVBox);

  Gtk::Entry *input = Gtk::manage(new Gtk::Entry());
  input->set_text("Enter answer here:");

  inputVBox->add(*input);
  input->grab_focus();
  input->set_size_request(get_width()/2);
  input->set_activates_default();

  ok->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this, &mainWindow::ok_clicked), input));

  vBox->show_all();
}

void mainWindow::makeMenuBar(Gtk::Box *vBox)
{
  Gtk::MenuBar *mainMenuBar = Gtk::manage(new Gtk::MenuBar());
  vBox->pack_start(*mainMenuBar, Gtk::PACK_SHRINK, 0);

  Gtk::MenuItem *crewTab = Gtk::manage(new Gtk::MenuItem("_Crew", true));
  mainMenuBar->append(*crewTab);
    Gtk::Menu *crewTabDrop = Gtk::manage(new Gtk::Menu());
    crewTab->set_submenu(*crewTabDrop);
      Gtk::MenuItem *crew_Zoro = Gtk::manage(new Gtk::MenuItem("Zoro", false));
      crewTabDrop->append(*crew_Zoro);
      Gtk::MenuItem *crew_Nami = Gtk::manage(new Gtk::MenuItem("Nami", false));
      crewTabDrop->append(*crew_Nami);
      Gtk::MenuItem *crew_Usopp = Gtk::manage(new Gtk::MenuItem("Usopp", false));
      crewTabDrop->append(*crew_Usopp);
      Gtk::MenuItem *crew_Sanji = Gtk::manage(new Gtk::MenuItem("Sanji", false));
      crewTabDrop->append(*crew_Sanji);
      Gtk::MenuItem *crew_Chopper = Gtk::manage(new Gtk::MenuItem("Chopper", false));
      crewTabDrop->append(*crew_Chopper);
      Gtk::MenuItem *crew_Robin = Gtk::manage(new Gtk::MenuItem("Robin", false));
      crewTabDrop->append(*crew_Robin);
      Gtk::MenuItem *crew_Franky = Gtk::manage(new Gtk::MenuItem("Franky", false));
      crewTabDrop->append(*crew_Franky);
      Gtk::MenuItem *crew_Brook = Gtk::manage(new Gtk::MenuItem("Brook", false));
      crewTabDrop->append(*crew_Brook);
      Gtk::MenuItem *crew_Jinbe = Gtk::manage(new Gtk::MenuItem("Jinbe", false));
      crewTabDrop->append(*crew_Jinbe);

  Gtk::MenuItem *familyTab = Gtk::manage(new Gtk::MenuItem("_Family", true));
  mainMenuBar->append(*familyTab);
    Gtk::Menu *familyTabDrop = Gtk::manage(new Gtk::Menu());
    familyTab->set_submenu(*familyTabDrop);
      Gtk::MenuItem *family_Garp = Gtk::manage(new Gtk::MenuItem("Grandfather: Monkey D. Garp", false));
      familyTabDrop->append(*family_Garp);
      Gtk::MenuItem *family_Dragon = Gtk::manage(new Gtk::MenuItem("Father: Monkey D. Dragon", false));
      familyTabDrop->append(*family_Dragon);

  Gtk::MenuItem *hintTab = Gtk::manage(new Gtk::MenuItem("_Hints", true));
  mainMenuBar->append(*hintTab);
    Gtk::Menu *hintTabDrop = Gtk::manage(new Gtk::Menu());
    hintTab->set_submenu(*hintTabDrop);
      Gtk::MenuItem *hint_1 = Gtk::manage(new Gtk::MenuItem("He only appears this way after using Gear 3", false));
      hintTabDrop->append(*hint_1);
      Gtk::MenuItem *hint_2 = Gtk::manage(new Gtk::MenuItem("Catch Phrase: \"I'm ganna be king of the pirates!\"", false));
      hintTabDrop->append(*hint_2);
}

void mainWindow::cancel_clicked()
{
  hide();
}

void mainWindow::ok_clicked(Gtk::Entry *input)
{
  string answer = input->get_text();
  if(answer.compare("Luffy") == 0 || answer.compare("luffy") == 0 ||  answer.compare("Monkey D. Luffy") == 0)
  {
    Gtk::MessageDialog correctAnswer("<b>You made the <span fgcolor='#0000ff'>correct</span> guess!!</b>", true, Gtk::MESSAGE_WARNING);
    correctAnswer.set_transient_for(*this);
    correctAnswer.set_default_response(Gtk::RESPONSE_OK);
    correctAnswer.run();
    hide();
  }
  else if(answer.compare("chibby luffy") == 0 || answer.compare("Chibby Luffy") == 0)
  {
    Gtk::MessageDialog superCorrectAnswer("<b>You made the best possible <span fgcolor='#0000ff'>correct</span> guess!!! \nCongrats Nakama!</b>", true, Gtk::MESSAGE_WARNING);
    superCorrectAnswer.set_transient_for(*this);
    superCorrectAnswer.set_default_response(Gtk::RESPONSE_OK);
    superCorrectAnswer.run();
    hide();
  }
  else
  {
    Gtk::MessageDialog wrongAnswer("<b>You answered <span fgcolor='#ff0000'>incorrectly</span>, Please try again!</b>", true, Gtk::MESSAGE_ERROR);
    wrongAnswer.set_transient_for(*this);
    wrongAnswer.set_default_response(Gtk::RESPONSE_OK);
    wrongAnswer.run();
  }
}
