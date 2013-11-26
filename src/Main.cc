/*******************************************************************************
 * Class Name: N/A - int main implementation
 * Date Created: November 17, 2013
 * Inheritance: none
 * Description: The main call implementation. Essentially just sets up
 *              necessary subsystems and starts up the application.
 ******************************************************************************/
#include "Application.h"
#include "Helpers.h"
#include "Md5.h"
#include "TinyXML.h"

bool initSDL()
{
  bool success = true;
  
  /* Initialize SDL */
  if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
  {
    std::cerr << "[ERROR] SDL could not initialize. SDL error: "
              << SDL_GetError() << std::endl;
    success = false;
  }
  
  /* Initialize SDL Image library */
  if(!IMG_Init(IMG_INIT_PNG))
  {
    std::cerr << "[ERROR] SDL_Image could not initialize. SDL_image error: "
              << IMG_GetError() << std::endl;
    success = false;
  }
    
  /* Initialize SDL TTF library */
  if(!TTF_WasInit() && TTF_Init() == -1)
  {
    std::cerr << "[ERROR] SDL_ttf could not initialize. SDL_ttf error: "
              << TTF_GetError() << std::endl;
    success = false;
  }

  /* Init SDL mixer, and open the audio with the chosen settings */
  int buffers = 1024; /* Size of audio buffers in memory */
  int channels = 2;   /* 1 channel = mono, 2 = stereo */
  int rate = 22050;   /* Frequency of Audio Playback */
  if(!Mix_Init(MIX_INIT_OGG) || 
     Mix_OpenAudio(rate, AUDIO_S16SYS, channels, buffers) != 0)
  {
    std::cerr << "[WARNING] Unable to initialize audio: " 
              << Mix_GetError() << std::endl;
  }

  return success;
}

int main(int argc, char** argv)
{
  (void)argc;
  (void)argv;
  
  /* The initial game seeding - for random number generation */
  Helpers::seed();
  
  /* Reading XML - testing */
  TinyXML2::XMLDocument doc;
  TinyXML2::XMLPrinter printer;
  if(!doc.LoadFile("maps/test_04"))
  {
    /* Node information */
    std::cout << "XML File Load Successful." << std::endl;
    TinyXML2::XMLNode* prime_node = doc.FirstChild()->NextSibling();
    TinyXML2::XMLNode* child_node = prime_node->FirstChild();
    TinyXML2::XMLNode* child2_node = child_node->FirstChild();
    std::cout << "Sequence information: " << prime_node->Value() << " " 
              << child_node->Value() << " " << child2_node->Value()
              << std::endl;
              
    /* Element information */
    TinyXML2::XMLElement* child_element = child_node->ToElement();
    TinyXML2::XMLElement* child2_element = child2_node->ToElement();
    std::cout << "Element: " << child_element->Name() << " \"" 
              << child_element->GetText() << "\" " << child2_element 
              << std::endl;
    
    /* Attribute information */
    const TinyXML2::XMLAttribute* child_attribute = 
                                                child_element->FirstAttribute();
    std::cout << "Attribute: " << child_attribute->Name() << " " 
              << child_attribute->Value() << std::endl;
    std::cout << "-------------------------------------------------------" 
              << std::endl;
    
    /* Output file information */
    doc.Print(&printer);
    std::string output(printer.CStr());
    std::vector<std::string> output_lines;
    Helpers::split(output, '\n', output_lines);
    for(uint16_t i = 0; i < output_lines.size(); i++)
      std::cout << output_lines[i] << std::endl;
    std::cout << "-------------------------------------------------------" 
              << std::endl;
  }

  /* Writing XML - testing */
  TinyXML2::XMLDocument doc2;
  TinyXML2::XMLElement* element = doc2.NewElement("game");
  doc2.InsertEndChild(element);
  TinyXML2::XMLElement* element2 = doc2.NewElement("battle");
  element2->SetAttribute("index", 4);
  //element2->SetValue("This is a fancy config");
  element->InsertEndChild(element2);
  element2->InsertEndChild(doc2.NewText("This is a fancy config"));
  doc2.InsertEndChild(doc2.NewElement("mini"));
  /* Output file information */
  TinyXML2::XMLPrinter printer2;
  doc2.Print(&printer2);
  std::string output2(printer2.CStr());
  std::vector<std::string> output2_lines;
  Helpers::split(output2, '\n', output2_lines);
  for(uint16_t i = 0; i < output2_lines.size(); i++)
    std::cout << output2_lines[i] << std::endl;
  std::cout << "-------------------------------------------------------" 
            << std::endl;

  /* Testing Md5 */
  std::cout << "MD5: " << MD5::compute("Hello sunshine") << std::endl;
  std::cout << "MD5: " << MD5::compute("Sarah Chou") << std::endl;
  std::cout << "-------------------------------------------------------" 
            << std::endl;
  
  /* Initialize SDL libraries */
  bool success = initSDL();
  
  if(success)
  {
    /* Create the application and start the run loop */
    Application game_app;
    if(game_app.initialize())
      game_app.run();
  
    /* Clean up the application, after the run loop is finished */
    game_app.uninitialize();
  }
  
  /* Clean up SDL libraries */
  Mix_CloseAudio();
  Mix_Quit();
  TTF_Quit();
  IMG_Quit();
  SDL_Quit();
  
	return 0;
}
