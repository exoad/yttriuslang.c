const { Discord, MessageEmbed } = require('discord.js');
const ready = require("../../configs/ready.json");

module.exports = async (bot) => {
  console.log(`${bot.user.tag} is now online.`)
  bot.user.setPresence({ activity: { name: ready.status }, status: 'online' })
}