/******************************************************************************\

                 This file is part of the Buildbotics firmware.

                   Copyright (c) 2015 - 2018, Buildbotics LLC
                              All rights reserved.

      This file ("the software") is free software: you can redistribute it
      and/or modify it under the terms of the GNU General Public License,
       version 2 as published by the Free Software Foundation. You should
       have received a copy of the GNU General Public License, version 2
      along with the software. If not, see <http://www.gnu.org/licenses/>.

      The software is distributed in the hope that it will be useful, but
           WITHOUT ANY WARRANTY; without even the implied warranty of
       MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
                Lesser General Public License for more details.

        You should have received a copy of the GNU Lesser General Public
                 License along with the software.  If not, see
                        <http://www.gnu.org/licenses/>.

                 For information regarding this software email:
                   "Joseph Coffland" <joseph@buildbotics.com>

\******************************************************************************/

'use strict'

const merge = require("lodash.merge");

const config_defaults = require("../resources/onefinity_defaults.json");

const variant_defaults = {
  machinist_x35: require("../resources/onefinity_machinist_x35_defaults.json"),
  woodworker_x35: require("../resources/onefinity_woodworker_x35_defaults.json"),
  woodworker_x50: require("../resources/onefinity_woodworker_x50_defaults.json"),
  journeyman_x50: require("../resources/onefinity_journeyman_x50_defaults.json")
};

const api = require('./api');

module.exports = {
  template: '#admin-general-view-template',
  props: ['config', 'state'],

  data: function () {
    return {
      confirmReset: false,
      autoCheckUpgrade: true,
      reset_variant: ''
    }
  },

  ready: function () {
    this.autoCheckUpgrade = this.config.admin['auto-check-upgrade']
  },

  methods: {
    backup: function () {
      document.getElementById('download-target').src = '/api/config/download';
    },

    restore_config: function () {
      // If we don't reset the form the browser may cache file if name is same
      // even if contents have changed
      $('.restore-config')[0].reset();
      $('.restore-config input').click();
    },

    restore: function (e) {
      const files = e.target.files || e.dataTransfer.files;
      if (!files.length) {
        return;
      }

      const fileReader = new FileReader();
      fileReader.onload = async ({ target }) => {
        let config;
        try {
          config = JSON.parse(target.result);
        } catch (ex) {
          api.alert("Invalid config file");
          return;
        }

        try {
          await api.put('config/save', config);
          this.$dispatch('update');
          SvelteComponents.showDialog("Message", { title: "Success", message: "Configuration restored" })
        } catch (error) {
          api.alert('Restore failed', error);
        }
      }

      fileReader.readAsText(files[0]);
    },

    reset: async function () {
      const config = merge(
        {},
        config_defaults,
        variant_defaults[this.reset_variant]
      );

      try {
        await api.put('config/save', config)
        this.confirmReset = false;
        this.$dispatch('update');
        SvelteComponents.showDialog("Message", { title: "Success", message: "Configuration restored" })
      } catch (err) {
        api.alert('Restore failed');
        console.error('Restore failed', err);
      }
    },

    check: function () {
      this.$dispatch('check')
    },

    upgrade: function () {
      this.$dispatch('upgrade')
    },

    upload_firmware: function () {
      // If we don't reset the form the browser may cache file if name is same
      // even if contents have changed
      $('.upload-firmware')[0].reset();
      $('.upload-firmware input').click();
    },

    upload: function (e) {
      var files = e.target.files || e.dataTransfer.files;
      if (!files.length) return;
      this.$dispatch('upload', files[0]);
    },

    change_auto_check_upgrade: function () {
      this.config.admin['auto-check-upgrade'] = this.autoCheckUpgrade;
      this.$dispatch('config-changed');
    }
  }
}
