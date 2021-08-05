const { Discord, MessageEmbed } = require('discord.js');
const ready = require("../../configs/ready.json");
const sysinfo = require("../../sysinfo.json");

module.exports = async (bot) => {
  console.log(`${bot.user.tag} is now online.`)
  console.log(`Platformo: ${sysinfo.os}\nSystem: ${sysinfo.system}`);
  bot.user.setPresence({ activity: { name: ready.status }, status: 'online' })
}