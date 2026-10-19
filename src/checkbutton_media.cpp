#include "checkbutton_media.h"

#include <gtkmm/image.h>
#include "GIFImage.h"

CheckbuttonMedia::CheckbuttonMedia(CheckbuttonMedia::CheckbuttonMediaType media_type, std::string path_to_media):
    AspectFrame(0.5,0.5){
    path_to_media_ = path_to_media;
    if ( media_type == CheckbuttonMediaType::IMAGE) {
        widget_= new Gtk::Picture(path_to_media);
        widget_->set_halign(Gtk::Align::FILL);
        widget_->set_valign(Gtk::Align::FILL);
        widget_->set_hexpand(true);
        widget_->set_vexpand(true);
        static_cast<Gtk::Picture *>(widget_)->set_content_fit(Gtk::ContentFit::FILL);
    } else if ( media_type == CheckbuttonMediaType::GIF) {
        widget_= new GIFImage(const_cast<char *>(path_to_media.c_str()));
    }else if ( media_type == CheckbuttonMediaType::MP4) {
        auto video = Gtk::MediaFile::create_for_filename(path_to_media);
        video->set_loop(true);
        video->play();
        video->set_muted(true);
        widget_ = new Gtk::Picture();
        widget_->set_halign(Gtk::Align::FILL);
        widget_->set_valign(Gtk::Align::FILL);
        widget_->set_hexpand(true);
        widget_->set_vexpand(true);
        static_cast<Gtk::Picture *>(widget_)->set_content_fit(Gtk::ContentFit::FILL);
        static_cast<Gtk::Picture *>(widget_)->set_paintable(video);
    }

    gesture_click_ = Gtk::GestureClick::create();
    gesture_click_->signal_pressed().connect(sigc::mem_fun(*this, &CheckbuttonMedia::ToggleSelection));

    overlay_ = Gtk::Overlay();
    checkbutton_ = Gtk::CheckButton();
    checkbutton_.set_halign(Gtk::Align::START);
    checkbutton_.set_valign(Gtk::Align::START);
    set_hexpand(true);
    set_vexpand(true);

    overlay_.set_child(*widget_);
    overlay_.add_overlay(checkbutton_);

    widget_->add_controller(gesture_click_);
    set_child(overlay_);
}
std::string CheckbuttonMedia::GetGIFImageVersion() {
    return GIFIMAGE_VERSION;
}
void CheckbuttonMedia::ToggleSelection(int press, double x, double y) {
    checkbutton_.set_active(!checkbutton_.get_active());
}
std::string CheckbuttonMedia::GetResourcePath() {
    return path_to_media_;
}
bool CheckbuttonMedia::IsSelected() {
    return checkbutton_.get_active();
}
std::string CheckbuttonMedia::GetVersion() {
    return CHECKBUTTON_MEDIA_VERSION;
}
