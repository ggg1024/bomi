#ifndef MRLSTATE_HPP
#define MRLSTATE_HPP

#include "stdafx.hpp"
#include "mrl.hpp"
#include "enum/clippingmethod.hpp"
#include "enum/interpolatortype.hpp"
#include "enum/videoratio.hpp"
#include "enum/verticalalignment.hpp"
#include "enum/horizontalalignment.hpp"
#include "enum/colorrange.hpp"
#include "enum/deintmode.hpp"
#include "enum/dithering.hpp"
#include "enum/channellayout.hpp"
#include "enum/subtitledisplay.hpp"
#include "enum/videoeffect.hpp"
#include "video/videocolor.hpp"
#include "subtitle/submisc.hpp"

class MrlStateProperty;
static inline bool operator == (const QMetaProperty &lhs, const QMetaProperty &rhs) {
    return lhs.enclosingMetaObject() == rhs.enclosingMetaObject() && lhs.propertyIndex() == rhs.propertyIndex();
}

class MrlStateV2 : public QObject {
    Q_OBJECT
    Q_PROPERTY(Mrl mrl MEMBER mrl)
    Q_PROPERTY(QString device MEMBER device)
    Q_PROPERTY(QDateTime last_played_date_time MEMBER last_played_date_time)
    Q_PROPERTY(int resume_position MEMBER resume_position)
    Q_PROPERTY(int edition MEMBER edition)
    Q_PROPERTY(int play_speed MEMBER play_speed NOTIFY playSpeedChanged)

    Q_PROPERTY(InterpolatorType video_interpolator MEMBER video_interpolator NOTIFY videoInterpolatorChanged)
    Q_PROPERTY(InterpolatorType video_chroma_upscaler MEMBER video_chroma_upscaler NOTIFY videoChromaUpscalerChanged)
    Q_PROPERTY(VideoRatio video_aspect_ratio MEMBER video_aspect_ratio NOTIFY videoAspectRatioChanged)
    Q_PROPERTY(VideoRatio video_crop_ratio MEMBER video_crop_ratio NOTIFY videoCropRatioChanged)
    Q_PROPERTY(DeintMode video_deinterlacing MEMBER video_deinterlacing NOTIFY videoDeinterlacingChanged)
    Q_PROPERTY(Dithering video_dithering MEMBER video_dithering NOTIFY videoDitheringChanged)
    Q_PROPERTY(QPoint video_offset MEMBER video_offset NOTIFY videoOffsetChanged)
    Q_PROPERTY(VerticalAlignment video_vertical_alignment MEMBER video_vertical_alignment NOTIFY videoVerticalAlignmentChanged)
    Q_PROPERTY(HorizontalAlignment video_horizontal_alignment MEMBER video_horizontal_alignment NOTIFY videoHorizontalAlignmentChanged)
    Q_PROPERTY(VideoColor video_color MEMBER video_color NOTIFY videoColorChanged)
    Q_PROPERTY(ColorRange video_range MEMBER video_range NOTIFY videoRangeChanged)

    Q_PROPERTY(int audio_volume MEMBER audio_volume NOTIFY audioVolumeChanged)
    Q_PROPERTY(int audio_amplifier MEMBER audio_amplifier NOTIFY audioAmpChanged)
    Q_PROPERTY(int audio_sync MEMBER audio_sync NOTIFY audioSyncChanged REVISION 1)
    Q_PROPERTY(int audio_track MEMBER audio_track REVISION 1)
    Q_PROPERTY(bool audio_muted MEMBER audio_muted NOTIFY audioMutedChanged)
    Q_PROPERTY(bool audio_volume_normalizer MEMBER audio_volume_normalizer NOTIFY audioVolumeNormalizerChanged)
    Q_PROPERTY(bool audio_tempo_scaler MEMBER audio_tempo_scaler NOTIFY audioTempoScalerChanged)
    Q_PROPERTY(ChannelLayout audio_channel_layout MEMBER audio_channel_layout NOTIFY audioChannelLayoutChanged)

    Q_PROPERTY(VerticalAlignment sub_alignment MEMBER sub_alignment NOTIFY subAlignmentChanged)
    Q_PROPERTY(SubtitleDisplay sub_display MEMBER sub_display NOTIFY subDisplayChanged)
    Q_PROPERTY(int sub_position MEMBER sub_position NOTIFY subPositionChanged)
    Q_PROPERTY(int sub_sync MEMBER sub_sync NOTIFY subSyncChanged REVISION 1)
    Q_PROPERTY(SubtitleStateInfo sub_track MEMBER sub_track REVISION 1)
public:
    Mrl mrl;
    QString device;

// play state
    QDateTime last_played_date_time;
    int resume_position = 0;
    int play_speed = 100;
    int edition = -1;

// video state
    VideoRatio video_aspect_ratio = VideoRatio::Source;
    VideoRatio video_crop_ratio = VideoRatio::Source;
    DeintMode video_deinterlacing = DeintMode::Auto;
    InterpolatorType video_interpolator = InterpolatorType::Bilinear;
    InterpolatorType video_chroma_upscaler = InterpolatorType::Bilinear;
    Dithering video_dithering = Dithering::Fruit;
    QPoint video_offset = {0, 0};
    VerticalAlignment video_vertical_alignment = VerticalAlignment::Center;
    HorizontalAlignment video_horizontal_alignment = HorizontalAlignment::Center;
    VideoColor video_color = {0, 0, 0, 0};
    ColorRange video_range = ColorRange::Auto;
    VideoEffects video_effects = 0;

// audio state
    int audio_amplifier = 100;
    int audio_volume = 100, audio_sync = 0;
    bool audio_muted = false, audio_volume_normalizer = false, audio_tempo_scaler = true;
    ChannelLayout audio_channel_layout = ChannelLayoutInfo::default_();
    int audio_track = -1;

// subtitle state
    int sub_position = 100;
    int sub_sync = 0;
    SubtitleDisplay sub_display = SubtitleDisplay::OnLetterbox;
    VerticalAlignment sub_alignment = VerticalAlignment::Bottom;
    SubtitleStateInfo sub_track;

    static const int Version = 2;

    static QList<MrlStateProperty> restorableProperties();
signals:
    void playSpeedChanged();

    void videoInterpolatorChanged();
    void videoAspectRatioChanged();
    void videoCropRatioChanged();
    void videoDeinterlacingChanged();
    void videoChromaUpscalerChanged();
    void videoDitheringChanged();
    void videoColorChanged(const VideoColor &color);
    void videoOffsetChanged();
    void videoVerticalAlignmentChanged();
    void videoHorizontalAlignmentChanged();
    void videoRangeChanged();

    void audioVolumeChanged();
    void audioAmpChanged();
    void audioMutedChanged();
    void audioSyncChanged();
    void audioVolumeNormalizerChanged();
    void audioTempoScalerChanged();
    void audioChannelLayoutChanged();
    void audioTrackChanged();

    void subPositionChanged();
    void subSyncChanged();
    void subDisplayChanged();
    void subAlignmentChanged();
};

using MrlState = MrlStateV2;

class MrlStateProperty {
public:
    MrlStateProperty() {}
    bool operator == (const MrlStateProperty &rhs) const {
        return m_property.propertyIndex() == rhs.m_property.propertyIndex();
    }
    auto info() const -> QString { return m_info; }
    const QMetaProperty &property() const { return m_property; }
private:
    MrlStateProperty(const QMetaProperty &p, const QString &info)
    : m_property(p), m_info(info) {}
    QMetaProperty m_property;
    QString m_info;
    friend class MrlStateV2;
};

struct MrlField {
    auto type() const -> QString { return m_type; }
    auto toSql(const QVariant &var) const -> QString { return m_toSql(var); }
    auto fromSql(const QVariant &var) const -> QVariant { return m_fromSql(var, m_property.userType()); }
    const QMetaProperty &property() const { return m_property; }
    const QVariant &default_() const { return m_default; }
    static QList<MrlField> list();
private:
    static auto pass(const QVariant &var, const QVariant &def) -> QVariant { return var.isNull() ? def : var; }
    QString m_type;
    QMetaProperty m_property;
    QVariant m_default;
    QString (*m_toSql)(const QVariant&);
    QVariant (*m_fromSql)(const QVariant&, const QVariant&) = pass;
};

namespace MrlStateHelpers {

enum BitHelper {
    B_MSec = 10,
    B_Sec = 6,
    B_Min = 6,
    B_Hour = 5,
    B_Day = 5,
    B_Month = 4,
    B_Year = 14
};

enum ShiftHelper {
    S_MSec = 0,
    S_Sec = S_MSec + B_MSec,
    S_Min = S_Sec + B_Sec,
    S_Hour = S_Min + B_Min,
    S_Day = S_Hour + B_Hour,
    S_Month = S_Day + B_Day,
    S_Year = S_Month + B_Month
};

static inline auto _ToSql(const QString &text) -> QString {
    return _L('\'') % QString(text).replace(_L('\''), _L("''")) % _L('\'');
}


static inline auto _ToSql(const QDateTime &dt) -> QString {
    const auto date = dt.date();
    const auto time = dt.time();
#define CV(v, s) (qint64(v) << S_##s)
    qint64 res = 0x0;
    res |= CV(date.year(), Year);
    res |= CV(date.month(), Month);
    res |= CV(date.day(), Day);
    res |= CV(time.hour(), Hour);
    res |= CV(time.msec(), MSec);
    res |= CV(time.second(), Sec);
    res |= CV(time.minute(), Min);
    return QString::number(res);
#undef CV
}

static inline auto _DateTimeFromSql(qint64 dt) -> QString {
#define XT(s) ((dt >> S_##s) & ((1 << B_##s)-1))
    const QString q("%1");
    auto pad = [&q] (int v, int n) -> QString { return q.arg(v, n, 10, _L('0')); };
    return pad(XT(Year), 4) % _L('/') % pad(XT(Month), 2) % _L('/') % pad(XT(Day), 2) % _L(' ')
            % pad(XT(Hour), 2) % _L(':') % pad(XT(Min), 2) % _L(':') % pad(XT(Sec), 2);
#undef XT
}

static inline auto _ToSql(qint8 integer) -> QString { return QString::number(integer); }
static inline auto _ToSql(qint16 integer) -> QString { return QString::number(integer); }
static inline auto _ToSql(qint32 integer) -> QString { return QString::number(integer); }
static inline auto _ToSql(qint64 integer) -> QString { return QString::number(integer); }

static inline auto _ToSql(const QPoint &p) -> QString
{
    return _L('\'') % QString::number(p.x()) % ","
                    % QString::number(p.y()) % _L('\'');
}

static inline auto _ToSql(const QJsonObject &json) -> QString
{ return _ToSql(_JsonToString(json)); }

static inline auto _PointFromSql(const QString &str, const QPoint &def) -> QPoint {
    auto index = str.indexOf(',');
    if (index < 0)
        return def;
    bool ok1 = false, ok2 = false;
    QPoint ret{str.midRef(0, index).toInt(&ok1), str.midRef(index+1).toInt(&ok2)};
    return ok1 && ok2 ? ret : def;
}

std::tuple<MrlState*, QList<MrlState*>> _ImportMrlStatesFromPreviousVersion(int version, QSqlDatabase db);

template<class F>
static auto _MrlFieldColumnListString(const QList<F> &list) -> QString {
    QString columns;
    for (auto &f : list)
        columns += f.property().name() % _L(", ");
    columns.chop(2);
    return columns;
}

template<class F>
static auto _MrlFieldValueListString(const QList<F> &list, const QObject *state) -> QString {
    QString values;
    for (auto &f : list)
        values += f.toSql(f.property().read(state)) % _L(", ");
    values.chop(2);
    return values;
}

//    QString q = _L("INSERT OR REPLACE INTO app (id, ") % columns % _L(") VALUES (0, %1)");
static inline auto _InsertMrlState(QSqlQuery &query, const QList<MrlField> &fields, const MrlState *state, const QString &queryTemplate) -> bool {
    return query.exec(queryTemplate.arg(_MrlFieldValueListString(fields, state)));
}

static inline auto _MakeInsertQueryTemplate(const QString &tableName, const QList<MrlField> &fields) -> QString {
    return QString::fromLatin1("INSERT OR REPLACE INTO %1 (%2) VALUES (%3)").arg(tableName).arg(_MrlFieldColumnListString(fields));
}

static inline auto _MakeInsertQuery(const QString &table, const QList<MrlField> &fields, const MrlState *state) -> QString {
    return _MakeInsertQueryTemplate(table, fields).arg(_MrlFieldValueListString(fields, state));
}


template<class T = MrlState, class F>
auto _FillMrlStateFromRecord(T *state, const QList<F> &fields, const QSqlRecord &record) -> void {
    for (int i=0; i<fields.size(); ++i) {
        const auto &f = fields[i];
        const QMetaProperty p = f.property();
        Q_ASSERT(p.name() == record.fieldName(i));
        p.write(state, f.fromSql(record.value(i)));
    }
}

template<class T = MrlState, class F>
T *_MakeMrlStateFromQuery(const QList<F> &fields, const QSqlQuery &query) {
    auto state = new T;
    _FillMrlStateFromRecord(state, fields, query);
    return state;
}

}

#endif // MRLSTATE_HPP