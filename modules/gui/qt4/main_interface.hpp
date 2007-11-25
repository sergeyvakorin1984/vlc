/*****************************************************************************
 * main_interface.hpp : Main Interface
 ****************************************************************************
 * Copyright (C) 2006-2007 the VideoLAN team
 * $Id$
 *
 * Authors: Clément Stenac <zorglub@videolan.org>
 *          Jean-Baptiste Kempf <jb@videolan.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

#ifndef _MAIN_INTERFACE_H_
#define _MAIN_INTERFACE_H_

#include "qt4.hpp"
#include "util/qvlcframe.hpp"
#include "components/preferences_widgets.hpp"

#include <vlc_aout.h>

#include <QSystemTrayIcon>

class QSettings;
class QCloseEvent;
class QKeyEvent;
class QLabel;
class QEvent;
class InputManager;
class VideoWidget;
class BackgroundWidget;
class PlaylistWidget;
class VisualSelector;
class AdvControlsWidget;
class ControlsWidget;
class SpeedControlWidget;
class QMenu;
class QSize;
class QDockWidet;

enum{
    CONTROLS_HIDDEN = 0x0,
    CONTROLS_VISIBLE = 0x1,
    CONTROLS_ADVANCED = 0x2
};

class MainInterface : public QVLCMW
{
    Q_OBJECT;
public:
    MainInterface( intf_thread_t *);
    virtual ~MainInterface();

    void *requestVideo( vout_thread_t *p_nvout, int *pi_x,
                        int *pi_y, unsigned int *pi_width,
                        unsigned int *pi_height );
    void releaseVideo( void * );
    int controlVideo( void *p_window, int i_query, va_list args );

    QSystemTrayIcon *getSysTray() { return sysTray; };
    QMenu *getSysTrayMenu() { return systrayMenu; };
    int getControlsVisibilityStatus();
    QSize sizeHint() const;
protected:
//    void resizeEvent( QResizeEvent * );
    void dropEvent( QDropEvent *);
    void dragEnterEvent( QDragEnterEvent * );
    void dragMoveEvent( QDragMoveEvent * );
    void dragLeaveEvent( QDragLeaveEvent * );
    void closeEvent( QCloseEvent *);

    friend class VolumeClickHandler;
private:
    QSettings           *settings;
    QSize                mainSize, addSize;
    QSystemTrayIcon     *sysTray;
    QMenu               *systrayMenu;
    QString              input_name;
    QVBoxLayout         *mainLayout;
    ControlsWidget      *controls;
    QMenu                *speedControlMenu;
    SpeedControlWidget  *speedControl;

    bool                 need_components_update;

    void handleMainUi( QSettings* );
    void handleSystray();

    void createSystray();
    void privacyDialog( QList<ConfigControl *> controls );

    /* Video */
    VideoWidget         *videoWidget;
    virtual void keyPressEvent( QKeyEvent *);
    virtual void wheelEvent( QWheelEvent * );

    bool                 embeddedPlaylistWasActive;
    bool                 videoIsActive;
    QSize                savedVideoSize;

    BackgroundWidget    *bgWidget;
    VisualSelector      *visualSelector;
    PlaylistWidget      *playlistWidget;
    QDockWidget         *dockPL;

    bool                 videoEmbeddedFlag;
    bool                 alwaysVideoFlag;
    bool                 visualSelectorEnabled;
    bool                 notificationEnabled;

    InputManager        *main_input_manager;
    input_thread_t      *p_input;    ///< Main input associated to the playlist

    /* Status Bar */
    QLabel              *timeLabel;
    QLabel              *speedLabel;
    QLabel              *nameLabel;

    bool                 b_remainingTime; /* showing elapsed or remaining time */

    void customEvent( QEvent *);
public slots:
    void undockPlaylist();
    void toggleMinimalView();
    void togglePlaylist();
    void toggleUpdateSystrayMenu();
    void toggleAdvanced();

    /* Manage the Video Functions from the vout threads */
    void releaseVideoSlot( void * );

private slots:
    void debug();
    void updateOnTimer();
    void doComponentsUpdate();
    void setStatus( int );
    void setRate( int );
    void setName( QString );
    void setVLCWindowsTitle( QString title = "" );
    void setDisplayPosition( float, int, int );
    void toggleTimeDisplay();
#if 0
    void visual();
#endif
    void handleSystrayClick( QSystemTrayIcon::ActivationReason );
    void updateSystrayTooltipName( QString );
    void updateSystrayTooltipStatus( int );
    void showSpeedMenu( QPoint );
signals:
    void askReleaseVideo( void * );
    void askVideoToToggle();
    void askBgWidgetToToggle();
};

#endif
