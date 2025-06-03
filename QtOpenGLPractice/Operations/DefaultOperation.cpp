#include "Operations/Operation.h"
#include "Operations/DemoOperation.h"
#include "Operations/EditVectorFontOperation.h"
#include "Operations/UnicodeOutlineOperation.h"
#include "Operations/FBOOperation.h"

Operation* createDefaultOperation(const std::string &name)
{
    if (name == "FirstTriangle")
    {
        FirstTriangle* ft = new FirstTriangle;
        return ft;
    }
    else if (name == "FontOperation")
    {
        FontOperation* font = new FontOperation;
        return font;
    }
    else if (name == "MillionPrimitiveOperation")
    {
        MillionPrimitiveOperation* million = new MillionPrimitiveOperation;
        return million;
    }
    else if (name == "EditVectorFont")
    {
        EditVectorFont* edit_font = new EditVectorFont;
        return edit_font;
    }
    else if (name == "UnicodeOutlineOperation")
    {
        UnicodeOutlineOperation* font_outline = new UnicodeOutlineOperation;
        return font_outline;
    }
    else if (name == "FBOOperation")
    {
        FBOOperation* fbo_opr = new FBOOperation;
        return fbo_opr;
    }

    return new FirstTriangle;

}