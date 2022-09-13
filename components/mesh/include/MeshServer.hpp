//
// Created by jadjer on 13.09.22.
//

#pragma once

#include <esp_ble_mesh_config_model_api.h>

class MeshServer {
 public:
  explicit MeshServer(uint8_t const* uuid);
  ~MeshServer();

  esp_err_t start();

 private:
  esp_ble_mesh_prov_t *m_provision;
  esp_ble_mesh_comp_t * m_composition;
  esp_ble_mesh_cfg_srv_t *m_configServer;

 private:
  void provisioningCallback(esp_ble_mesh_prov_cb_event_t event, esp_ble_mesh_prov_cb_param_t* param);
};
