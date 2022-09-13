//
// Created by jadjer on 13.09.22.
//

#include "MeshServer.hpp"

#include <functional>
#include <esp_ble_mesh_common_api.h>
#include <esp_ble_mesh_provisioning_api.h>

#define TAG "EXAMPLE"

MeshServer::MeshServer(uint8_t const* uuid) {
  m_provision = new esp_ble_mesh_prov_t{
      .uuid = uuid,
  };

  m_composition = new esp_ble_mesh_comp_t{
      .cid = 0x02E5,
      .element_count = ARRAY_SIZE(elements),
      .elements = elements,
  };

  m_configServer = new esp_ble_mesh_cfg_srv_t{
      .net_transmit = ESP_BLE_MESH_TRANSMIT(2, 20),
      .relay = ESP_BLE_MESH_RELAY_ENABLED,
      .relay_retransmit = ESP_BLE_MESH_TRANSMIT(2, 20),
      .beacon = ESP_BLE_MESH_BEACON_ENABLED,
      .gatt_proxy = ESP_BLE_MESH_GATT_PROXY_ENABLED,
      .friend_state = ESP_BLE_MESH_FRIEND_ENABLED,
      .default_ttl = 7,
  };
}

MeshServer::~MeshServer() {
  delete m_configServer;
}

esp_err_t MeshServer::start() {
  esp_err_t err;

  err = esp_ble_mesh_register_prov_callback(std::function<void (* esp_ble_mesh_prov_cb_t)>(provisioningCallback));
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Failed to initialize mesh stack");
    return err;
  }

  err = esp_ble_mesh_register_config_server_callback(example_ble_mesh_config_server_cb);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Failed to initialize mesh stack");
    return err;
  }

  err = esp_ble_mesh_register_sensor_server_callback(example_ble_mesh_sensor_server_cb);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Failed to initialize mesh stack");
    return err;
  }

  err = esp_ble_mesh_init(m_provision, m_composition);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Failed to initialize mesh stack");
    return err;
  }

  err = esp_ble_mesh_node_prov_enable(static_cast<esp_ble_mesh_prov_bearer_t>(ESP_BLE_MESH_PROV_ADV | ESP_BLE_MESH_PROV_GATT));
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Failed to enable mesh node");
    return err;
  }

  //  board_led_operation(LED_G, LED_ON);

  ESP_LOGI(TAG, "BLE Mesh sensor server initialized");

  return ESP_OK;
}

void MeshServer::provisioningCallback(esp_ble_mesh_prov_cb_event_t event, esp_ble_mesh_prov_cb_param_t* param) {
  switch (event) {
    case ESP_BLE_MESH_PROV_REGISTER_COMP_EVT:
      ESP_LOGI(TAG, "ESP_BLE_MESH_PROV_REGISTER_COMP_EVT, err_code %d", param->prov_register_comp.err_code);
      break;
    case ESP_BLE_MESH_NODE_PROV_ENABLE_COMP_EVT:
      ESP_LOGI(TAG, "ESP_BLE_MESH_NODE_PROV_ENABLE_COMP_EVT, err_code %d", param->node_prov_enable_comp.err_code);
      break;
    case ESP_BLE_MESH_NODE_PROV_LINK_OPEN_EVT:
      ESP_LOGI(TAG, "ESP_BLE_MESH_NODE_PROV_LINK_OPEN_EVT, bearer %s",
               param->node_prov_link_open.bearer == ESP_BLE_MESH_PROV_ADV ? "PB-ADV" : "PB-GATT");
      break;
    case ESP_BLE_MESH_NODE_PROV_LINK_CLOSE_EVT:
      ESP_LOGI(TAG, "ESP_BLE_MESH_NODE_PROV_LINK_CLOSE_EVT, bearer %s",
               param->node_prov_link_close.bearer == ESP_BLE_MESH_PROV_ADV ? "PB-ADV" : "PB-GATT");
      break;
    case ESP_BLE_MESH_NODE_PROV_COMPLETE_EVT:
      ESP_LOGI(TAG, "ESP_BLE_MESH_NODE_PROV_COMPLETE_EVT");
      prov_complete(param->node_prov_complete.net_idx, param->node_prov_complete.addr, param->node_prov_complete.flags, param->node_prov_complete.iv_index);
      break;
    case ESP_BLE_MESH_NODE_PROV_RESET_EVT:
      ESP_LOGI(TAG, "ESP_BLE_MESH_NODE_PROV_RESET_EVT");
      break;
    case ESP_BLE_MESH_NODE_SET_UNPROV_DEV_NAME_COMP_EVT:
      ESP_LOGI(TAG, "ESP_BLE_MESH_NODE_SET_UNPROV_DEV_NAME_COMP_EVT, err_code %d", param->node_set_unprov_dev_name_comp.err_code);
      break;
    default:
      break;
  }
}
