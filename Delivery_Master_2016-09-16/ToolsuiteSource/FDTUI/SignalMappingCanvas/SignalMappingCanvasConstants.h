#ifndef SIGNALMAPPINGCANVASCONSTANTS
#define SIGNALMAPPINGCANVASCONSTANTS

class QFont;

#define CANVAS_PADDING                  (10)
#define IOBLOCK_WIDTH                   (175)
#define IOBLOCK_TO_CENTERBLOCK_GAP      (150)
#define IOBLOCK_TO_IOBLOCK_VERTICAL_GAP (50)
#define IOBLOCK_TITLE_HEIGHT            (30)
#define IOBLOCK_SECTION_GAP             (30)
#define CENTERBLOCK_WIDTH               (250)
#define CENTERBLOCK_SECTION_GAP         (30)
#define CENTERBLOCK_TITLE_HEIGHT        (30)
#define HEIGHT_PER_CONNECTOR            (50)

class SignalMappingCanvasConstants
{
public:
    static QFont DefaultFont;
    static QFont CenterBlockTitleFont;
    static QFont IOBlockTitleFont;
    static QFont SignalNameFont;
    static QFont SmallDebugFont;

private:
    SignalMappingCanvasConstants(void);
};

#endif // SIGNALMAPPINGCANVASCONSTANTS

