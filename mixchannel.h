#ifndef MIXCHANNEL_H
#define MIXCHANNEL_H

#include <QObject>
#include <QHBoxLayout>
#include <QSlider>
#include <QDial>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>

class MixChannel : public QObject
{
  Q_OBJECT
public:

  explicit MixChannel(QObject *parent = nullptr);
  QHBoxLayout* main_layout;

private:

  QVBoxLayout* _head_layout;
  QGridLayout* _body_layout;

  QLabel  *_lbl_g;
  QLabel  *_lbl_f;
  QLabel  *_lbl_pan;
  QLabel  *_lbl_duty;
  QLabel  *_lbl_wave;

  QDial   *_dia_pan;
  QSlider *_sli_g;
  QSlider *_sli_f;

  QLineEdit *_qle_p;
  QLineEdit *_qle_g;
  QLineEdit *_qle_f;
  QLineEdit *_qle_d;

  QComboBox *_cmb_wave;
  QSlider   *_sli_duty;

signals:

public slots:

private slots:
  void when_pan0_changed(int value);
  void when_pan1_changed();
  void when_gain0_changed();
  void when_gain1_changed();
  void when_freq0_changed();
  void when_freq1_changed();
  void when_wave_changed();
  void when_duty0_changed();
  void when_duty1_changed();
};

#endif
