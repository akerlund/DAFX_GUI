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

  explicit MixChannel(int id, QObject *parent = nullptr);
  QHBoxLayout* main_layout;

private:

  int _id;

  QVBoxLayout* _head_layout;
  QGridLayout* _body_layout;

  QLabel  *_lbl_gain;
  QLabel  *_lbl_freq;
  QLabel  *_lbl_pan;
  QLabel  *_lbl_duty;
  QLabel  *_lbl_wave;

  QDial   *_dia_pan;
  QSlider *_sli_gain;
  QSlider *_sli_freq;

  QLineEdit *_qle_pan;
  QLineEdit *_qle_gain;
  QLineEdit *_qle_freq;
  QLineEdit *_qle_duty;

  QComboBox *_cmb_wave;
  QSlider   *_sli_duty;

signals:
  void pan_value_changed(int id, int value);
  void gain_value_changed(int id, int value);
  void freq_value_changed(int id, int value);
  void wave_value_changed(int id, int value);
  void duty_value_changed(int id, int value);

public slots:

private slots:
  void when_pan0_changed(int value);
  void when_pan1_changed();
  void when_gain0_changed(int value);
  void when_gain1_changed();
  void when_freq0_changed(int value);
  void when_freq1_changed();
  void when_wave_changed(int value);
  void when_duty0_changed(int value);
  void when_duty1_changed();
};

#endif
