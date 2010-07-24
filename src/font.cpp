#include "graphiteng/font.h"
#include "GrFont.h"

using namespace org::sil::graphite::v2;

GrFont* IFont::makeGrFont(const GrFace *face) const			//this must stay alive all the time when the GrFont is alive. When finished with the GrFont, call IFont::destroyGrFace
{
    return new GrHintedFont(this, face);
}


/*static*/ GrFont* IFont::makeGrFont(float ppm, const GrFace *face)		//When finished with the GrFont, call IFont::destroyGrFace
{
    return new GrFont(ppm, face);
}


/*static*/ void IFont::destroyGrFont(GrFont *font)
{
    delete font;
}



