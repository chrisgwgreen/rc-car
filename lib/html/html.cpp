#include <html.h>
HTML::HTML() {}
const char *HTML::getHTML()
{
	return "<!DOCTYPE html>"
		   "<html>"
		   "<head>"
		   "  <meta charset=\"utf-8\">"
		   "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1, maximum-scale=1, user-scalable=0\" />"
		   "  <title>Breath</title>"
		   "  <link rel=\"stylesheet\" href=\"./css/index.css\">"
		   "</head>"
		   "<body>"
		   "  <div class=\"content-wrapper\">"
		   "    <div id=\"demoWheel\"></div>"
		   "    <label for=\"inhale\" class=\"label\">Inhale</label>"
		   "    <input id=\"inhale\" type=\"range\" name=\"inhale\" min=\"400\" max=\"5000\" step=\"1\" value=\"0\">"
		   "    <label for=\"inhale-pause\" class=\"label\">Inhale Pause</label>"
		   "    <input id=\"inhale-pause\" type=\"range\" name=\"inhale-pause\" min=\"0\" max=\"4000\" step=\"1\" value=\"0\">"
		   "    <label for=\"exhale\" class=\"label\">Exhale</label>"
		   "    <input id=\"exhale\" type=\"range\" name=\"exhale\" min=\"400\" max=\"5000\" step=\"1\" value=\"0\">"
		   "    <label for=\"exhale-pause\" class=\"label\">Exhale Pause</label>"
		   "    <input id=\"exhale-pause\" type=\"range\" name=\"exhale-pause\" min=\"0\" max=\"4000\" step=\"1\" value=\"0\">"
		   "  </div>div>"
		   "  <script src=\"./js/index.js\" charset=\"utf-8\"></script>"
		   "</body>"
		   "</html>"
		   "";
}
