#include "utils/MimeType.hpp"

#include "utils/Logger.hpp"

namespace http {

MimeType::MimeType(void) {}

MimeType::MimeType(ft::file const& file) {
    m_ext = file.getExtension();
    m_type = getMimeType();
}

MimeType::operator std::string(void) const { return (m_type); }

bool MimeType::operator==(MimeType const& rhs) const {
    return (m_ext == rhs.m_ext);
}

std::string MimeType::getMimeType(void) {
    if (m_ext == ".html") { return ("text/html"); }
    if (m_ext == ".htm") { return ("text/html"); }
    if (m_ext == ".shtml") { return ("text/html"); }
    if (m_ext == ".css") { return ("text/css"); }
    if (m_ext == ".xml") { return ("text/xml"); }
    if (m_ext == ".gif") { return ("image/gif"); }
    if (m_ext == ".jpeg") { return ("image/jpeg"); }
    if (m_ext == ".jpg") { return ("image/jpeg"); }
    if (m_ext == ".js") { return ("application/javascript"); }
    if (m_ext == ".atom") { return ("application/atom+xml"); }
    if (m_ext == ".rss") { return ("application/rss+xml"); }
    if (m_ext == ".mml") { return ("text/mathml"); }
    if (m_ext == ".txt") { return ("text/plain"); }
    if (m_ext == ".jad") { return ("text/vnd.sun.j2me.app-descriptor"); }
    if (m_ext == ".wml") { return ("text/vnd.wap.wml"); }
    if (m_ext == ".htc") { return ("text/x-component"); }
    if (m_ext == ".png") { return ("image/png"); }
    if (m_ext == ".tif") { return ("image/tiff"); }
    if (m_ext == ".tiff") { return ("image/tiff"); }
    if (m_ext == ".wbmp") { return ("image/vnd.wap.wbmp"); }
    if (m_ext == ".ico") { return ("image/x-icon"); }
    if (m_ext == ".jng") { return ("image/x-jng"); }
    if (m_ext == ".bmp") { return ("image/x-ms-bmp"); }
    if (m_ext == ".svg") { return ("image/svg+xml"); }
    if (m_ext == ".svgz") { return ("image/svg+xml"); }
    if (m_ext == ".webp") { return ("image/webp"); }
    if (m_ext == ".woff") { return ("application/font-woff"); }
    if (m_ext == ".jar") { return ("application/java-archive"); }
    if (m_ext == ".war") { return ("application/java-archive"); }
    if (m_ext == ".ear") { return ("application/java-archive"); }
    if (m_ext == ".json") { return ("application/json"); }
    if (m_ext == ".hqx") { return ("application/mac-binhex40"); }
    if (m_ext == ".doc") { return ("application/msword"); }
    if (m_ext == ".pdf") { return ("application/pdf"); }
    if (m_ext == ".ps") { return ("application/postscript"); }
    if (m_ext == ".eps") { return ("application/postscript"); }
    if (m_ext == ".ai") { return ("application/postscript"); }
    if (m_ext == ".rtf") { return ("application/rtf"); }
    if (m_ext == ".m3u8") { return ("application/vnd.apple.mpegurl"); }
    if (m_ext == ".xls") { return ("application/vnd.ms-excel"); }
    if (m_ext == ".eot") { return ("application/vnd.ms-fontobject"); }
    if (m_ext == ".ppt") { return ("application/vnd.ms-powerpoint"); }
    if (m_ext == ".wmlc") { return ("application/vnd.wap.wmlc"); }
    if (m_ext == ".kml") { return ("application/vnd.google-earth.kml+xml"); }
    if (m_ext == ".kmz") { return ("application/vnd.google-earth.kmz"); }
    if (m_ext == ".7z") { return ("application/x-7z-compressed"); }
    if (m_ext == ".cco") { return ("application/x-cocoa"); }
    if (m_ext == ".jardiff") { return ("application/x-java-archive-diff"); }
    if (m_ext == ".jnlp") { return ("application/x-java-jnlp-file"); }
    if (m_ext == ".run") { return ("application/x-makeself"); }
    if (m_ext == ".pl") { return ("application/x-perl"); }
    if (m_ext == ".pm") { return ("application/x-perl"); }
    if (m_ext == ".prc") { return ("application/x-pilot"); }
    if (m_ext == ".pdb") { return ("application/x-pilot"); }
    if (m_ext == ".rar") { return ("application/x-rar-compressed"); }
    if (m_ext == ".rpm") { return ("application/x-redhat-package-manager"); }
    if (m_ext == ".sea") { return ("application/x-sea"); }
    if (m_ext == ".swf") { return ("application/x-shockwave-flash"); }
    if (m_ext == ".sit") { return ("application/x-stuffit"); }
    if (m_ext == ".tcl") { return ("application/x-tcl"); }
    if (m_ext == ".tk") { return ("application/x-tcl"); }
    if (m_ext == ".der") { return ("application/x-x509-ca-cert"); }
    if (m_ext == ".pem") { return ("application/x-x509-ca-cert"); }
    if (m_ext == ".crt") { return ("application/x-x509-ca-cert"); }
    if (m_ext == ".xpi") { return ("application/x-xpinstall"); }
    if (m_ext == ".xhtml") { return ("application/xhtml+xml"); }
    if (m_ext == ".xspf") { return ("application/xspf+xml"); }
    if (m_ext == ".zip") { return ("application/zip"); }
    if (m_ext == ".bin") { return ("application/octet-stream"); }
    if (m_ext == ".exe") { return ("application/octet-stream"); }
    if (m_ext == ".dll") { return ("application/octet-stream"); }
    if (m_ext == ".deb") { return ("application/octet-stream"); }
    if (m_ext == ".dmg") { return ("application/octet-stream"); }
    if (m_ext == ".iso") { return ("application/octet-stream"); }
    if (m_ext == ".img") { return ("application/octet-stream"); }
    if (m_ext == ".msi") { return ("application/octet-stream"); }
    if (m_ext == ".msp") { return ("application/octet-stream"); }
    if (m_ext == ".msm") { return ("application/octet-stream"); }
    if (m_ext == ".docx") {
        return ("application/"
                "vnd.openxmlformats-officedocument.wordprocessingml.document");
    }
    if (m_ext == ".xlsx") {
        return ("application/"
                "vnd.openxmlformats-officedocument.spreadsheetml.sheet");
    }
    if (m_ext == ".pptx") {
        return (
            "application/"
            "vnd.openxmlformats-officedocument.presentationml.presentation");
    }
    if (m_ext == ".mid") { return ("audio/midi"); }
    if (m_ext == ".midi") { return ("audio/midi"); }
    if (m_ext == ".kar") { return ("audio/midi"); }
    if (m_ext == ".mp3") { return ("audio/mpeg"); }
    if (m_ext == ".ogg") { return ("audio/ogg"); }
    if (m_ext == ".m4a") { return ("audio/x-m4a"); }
    if (m_ext == ".ra") { return ("audio/x-realaudio"); }
    if (m_ext == ".3gpp") { return ("video/3gpp"); }
    if (m_ext == ".3gp") { return ("video/3gpp"); }
    if (m_ext == ".ts") { return ("video/mp2t"); }
    if (m_ext == ".mp4") { return ("video/mp4"); }
    if (m_ext == ".mpeg") { return ("video/mpeg"); }
    if (m_ext == ".mpg") { return ("video/mpeg"); }
    if (m_ext == ".mov") { return ("video/quicktime"); }
    if (m_ext == ".webm") { return ("video/webm"); }
    if (m_ext == ".flv") { return ("video/x-flv"); }
    if (m_ext == ".m4v") { return ("video/x-m4v"); }
    if (m_ext == ".mng") { return ("video/x-mng"); }
    if (m_ext == ".asx") { return ("video/x-ms-asf"); }
    if (m_ext == ".asf") { return ("video/x-ms-asf"); }
    if (m_ext == ".wmv") { return ("video/x-ms-wmv"); }
    if (m_ext == ".avi") { return ("video/x-msvideo"); }
    return ("text/plain");
}

} // namespace http